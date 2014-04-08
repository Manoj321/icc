package page_ranking;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;


public class Server extends Thread
{
   private ServerSocket serverSocket;
   public static HashMap<String, String> title_map = new HashMap<String,String>();
   
   public Server(int port) throws IOException
   {
      serverSocket = new ServerSocket(port);
      serverSocket.setSoTimeout(0);;
   }

   public void run()
   {
      while(true)
      {
         try
         {
            System.out.println("Waiting for client on port " + serverSocket.getLocalPort() + "...");
           
            Socket server = serverSocket.accept();
            
            System.out.println("Just connected to "+ server.getRemoteSocketAddress());
            
            InputStreamReader in= new InputStreamReader(server.getInputStream());
            BufferedReader input= new BufferedReader(in);
            String query=input.readLine();
            System.out.println("******received query: " +query);
            
            DataOutputStream out = new DataOutputStream(server.getOutputStream());
            page_rank obj = new page_rank(query);
            ArrayList<String> result= obj.page();
            String toBeSent="";
            int num=0;
            String nextTitle="",title="";
            int nextLen=0;
            String url="";
            LinkedList ndcgList=new LinkedList();
            int numresults = (result.size()>6)? 6:result.size();
          
            for(int i=0;i<numresults-1;i++){
            	title= title_map.get(result.get(i));
            	nextTitle=title_map.get(result.get(i+1));
            	System.out.println("title "+ title);
            	System.out.println("nexttitle "+ nextTitle);
            	url=result.get(i);
            	if(title !=null && nextTitle!=null){
            		if(title.compareTo(nextTitle)==0){
            			url= (title.length()<nextTitle.length())?result.get(i):result.get(i+1);
            			System.out.println("here");
            			i+=2;
            			numresults+=2;
            		}
            	}
            	if(title==null) title="Title";
            	ndcgList.add(url);
            	//call NDCG reult
            	toBeSent+=url+"++";
            	toBeSent+=title.trim();
            	toBeSent+="||";
            	System.out.println("+++++++++"+toBeSent);
            }
//            String result= obj.setURL();
            
            NDCG_5 ndcgObj= new NDCG_5();
        	System.out.println(ndcgObj.calculate_ndcg(query.toLowerCase(),ndcgList));

            out.writeUTF(toBeSent);
            System.out.println("this was sent:-->>"+toBeSent);
            server.close();
            out.flush();
            out.close();
            
            
         }catch(SocketTimeoutException s)
         {
            System.out.println("Socket timed out!");
            break;
         }catch(IOException e)
         {
            e.printStackTrace();
            break;
         } catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      }
   }
   public static void main(String [] args) throws ClassNotFoundException, SQLException
   {
	   Class.forName("com.mysql.jdbc.Driver");
		Connection connect = DriverManager
				.getConnection("jdbc:mysql://localhost/crawlerindex?"
						+ "user=root&password=connect");
		connect.setAutoCommit(true);
	   PreparedStatement titleStatement = connect.prepareStatement("select * from title");
	   ResultSet resultSetTitle = titleStatement.executeQuery(); 
		

	   
	   while(resultSetTitle.next()){
		   title_map.put(resultSetTitle.getString("urls"),resultSetTitle.getString("title"));
	   }
//	   System.out.println("titlemap:"+ title_map.toString());
	  int port = Integer.parseInt("1234");
      try
      {
         Thread t = new Server(port);
         t.start();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}