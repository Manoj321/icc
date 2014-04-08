package page_ranking;


import java.io.IOException;
import java.net.URL;
import java.util.Scanner;
 
 
public class parseURL
{
    // Read from a URL and return the content in a String
    public static String readURLContent(String urlString) 
                                    throws IOException
    {
        URL url = new URL(urlString);
        Scanner scan = new Scanner(url.openStream());
 
        String content = new String();
        while (scan.hasNext())
            content += scan.nextLine();
        scan.close();
        return content;
    }
     
    // Find title within the HTML content
    public static String findTitle(String str)
    {
        String tagOpen = "<title>";
        String tagClose = "</title>";
         
        int begin = str.indexOf(tagOpen) + tagOpen.length();
        int end = str.indexOf(tagClose);
        return str.substring(begin, end);
    }
     
    public static void main(String[] args) throws IOException 
    {
        Scanner scan = new Scanner(System.in);
        String title;
        String urlString = "http://luci.ics.uci.edu/lightweight/bioFaculty/index.html";
        if(urlString != null)
        {
        	 String content = readURLContent(urlString);
             title = findTitle(content);
             	
        }
        else
        {
        	title = "";
        }
        System.out.println(title);
    }
}