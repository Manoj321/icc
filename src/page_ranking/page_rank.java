package page_ranking;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.SortedMap;

class Result
{
	double tfidf;
	int docn;
	double TD;
	String pos;
	String url;
	ResultSet result_set;
	Result(ResultSet resultset, page_rank pr) throws SQLException
	{
		tfidf = resultset.getDouble("tfidf");
		docn = resultset.getInt("docsn");
		TD = resultset.getDouble("TD");
		url=resultset.getString("urls");
		if(TD==0.0){
			TD= pr.update(url);
		}
		pos = resultset.getString("pos");
		
	}
	
}

public class page_rank {
	private Connection connect = null;
	private PreparedStatement word_query_statement = null;
	private  PreparedStatement docMagStatement=null;
	
	public  HashMap<String, Double> query_map = null;
	int Total_N = 199685;
	public  HashMap<String, HashMap<String, Result>> resultsetmap;
	public  HashMap<String, Double> score_map = null;
	public  String[] query_words = null;
	 ResultSet docmag=null;
	 
	public static String Query =null; 
	public  double update(String url) throws SQLException{
		double temp=0.0;
		docMagStatement.setString(1,url);
		docmag=docMagStatement.executeQuery();
		if(docmag.next())
			temp= docmag.getDouble("td");
		return temp;
	}
	
	private  HashMap<String, Double> sortByValues(HashMap<String, Double> map) { 
	       List<Double> list = new LinkedList(map.entrySet());
	       // Defined Custom Comparator here
	       Collections.sort(list, new Comparator() {
	            public int compare(Object o1, Object o2) {
	               return ((Comparable) ((Map.Entry) (o2)).getValue())
	                  .compareTo(((Map.Entry) (o1)).getValue());
	            }
	       });

	       // Here I am copying the sorted list in HashMap
	       // using LinkedHashMap to preserve the insertion order
	       HashMap sortedHashMap = new LinkedHashMap();
	       for (Iterator it = list.iterator(); it.hasNext();) {
	              Map.Entry entry = (Map.Entry) it.next();
	              sortedHashMap.put(entry.getKey(), entry.getValue());
	       } 
	       return sortedHashMap;
	  }
	
	public page_rank(String query) throws ClassNotFoundException, SQLException {
		try {
			Class.forName("com.mysql.jdbc.Driver");
			connect = DriverManager
					.getConnection("jdbc:mysql://localhost/crawlerindex?"
							+ "user=root&password=connect");
			connect.setAutoCommit(true);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		word_query_statement = connect
				.prepareStatement("Select urls,docsn,tfidf,td, pos from INDEX1 where word=? ORDER BY TFIDF DESC limit 200");
		
		docMagStatement = connect.prepareStatement("select * from docmag where urls=? limit 1");
		
		
		
		query_map = new HashMap<String, Double>();
		score_map = new HashMap<String, Double>();
		
		
		this.Query=query.replaceAll("[^a-zA-Z0-9'-]", " ")
				.replace(" +", " ");
		query_words = Query.split("\\s+");

		resultsetmap = new HashMap<String, HashMap<String, Result>>();
		
	}

	public ArrayList<String> get_search_urls()
			throws ClassNotFoundException, SQLException {
		String url;
		int docn, i;
		double freq, idf, tf, tfidf;
		ArrayList<String> url_list = new ArrayList<String>();


		for (i = 0; i < query_words.length; i++) 
		{
			if (query_map.containsKey(query_words[i])) 
			{
				freq = query_map.get(query_words[i]);
				freq = freq + 1;
				query_map.put(query_words[i], new Double(freq));
			} else {
				query_map.put(query_words[i], new Double(1.0));
			}
		}

		Iterator<String> It = query_map.keySet().iterator();
		String query_word;
		while(It.hasNext())
		{
			query_word = It.next().toString();
			word_query_statement.setString(1, query_word);
			ResultSet resultSet = word_query_statement.executeQuery();
			HashMap<String, Result> temp_obj = new HashMap<String, Result>();			
			while (resultSet.next()) 
			{
				url=resultSet.getString("urls");
				if(url.contains("rss"))continue;
				if(url.contains(".xml")) continue;
				
				Result res = new Result(resultSet,this);
				
				url = res.url;
				docn = resultSet.getInt("DOCSN");
				freq = query_map.get(query_word);
				tf = (1 + Math.log10(freq));
				idf = Math.log10((double) Total_N / (double) docn);
				tfidf = tf * idf;
				if (!query_map.containsKey(query_word))
					query_map.put(query_word, tfidf);
				if (!url_list.contains(url)) 
				{
					url_list.add(url);
				}
				temp_obj.put(url, res);
			}
			resultsetmap.put(query_word, temp_obj);
		}
		
		System.out.println("query map: "+query_map);
		Iterator<String> It1 = query_map.keySet().iterator();
		double net_score = 0;
		while(It1.hasNext())
		{
			String word = It1.next();
			Double score = query_map.get(word);
			net_score += score*score;	
		}
		Iterator<String> It2 = query_map.keySet().iterator();
		while(It2.hasNext())
		{
			String word = It2.next();
			Double score = query_map.get(word);
			score /= Math.sqrt(net_score);
			query_map.put(word, score);
		}
		
		return url_list;
	}

	public HashMap<String, Double> get_cosine_score_map(
			ArrayList<String> url_list) throws SQLException {
		HashMap<String, Result> temp_resultmap = null;
		String query_word, url;
		double score = 0, query_mag = 1;
		double tfidf_doc, tfidf_query, doc_mag;
		double temp_score;
		String first_result=null;
		for (int i = 0; i < query_words.length; i++) 
		{
			boolean once=true;
			query_word = query_words[i];
			for (int j = 0; j < url_list.size(); j++) 
			{
				url = url_list.get(j);
				
				//artificially increasing scores
				if(url.contains("~"+query_word)){
					score+=200;
					if(once){
					first_result=url.split("~")[0]+"~"+query_word+"/";
					System.out.println("first result "+url.split("~")[0]+"~"+query_word);
					score_map.put(first_result, (double) 1000);
					once=false;
					}
				}
				
								
				temp_resultmap = resultsetmap.get(query_word);	
				Iterator<String> It = temp_resultmap.keySet().iterator();
				while (It.hasNext()) 
				{
					String map_url = It.next().toString();
					Result res = temp_resultmap.get(map_url);
					if (map_url.compareTo(url) == 0)
					{
						tfidf_doc = res.tfidf;
						tfidf_query = query_map.get(query_word);
						score += tfidf_doc * tfidf_query;
						doc_mag = Math.sqrt((double)res.TD);
						score /= (doc_mag * query_mag);

						//add 1 to the query in title
						String title= Server.title_map.get(url);
						
						if(title!=null)	{	
									
								title=title.toLowerCase().replaceAll("[^a-zA-Z0-9'-]", " ").replace(" +", " ");
									if(title.contains(Query)){
										
										score+=1;
									}
						}

						
						if (score_map.containsKey(url))
						{
							temp_score = score_map.get(url);
							temp_score += score;
							score_map.put(url, new Double(temp_score));
						} else
						{
							score_map.put(url, new Double(score));
						}
					}
				}				
			}
		}
		System.out.println("scoremap"+ score_map.toString());
		return score_map;
	}

	public ArrayList<String> page() throws ClassNotFoundException,
			SQLException {
		BufferedReader reader = null;
		ArrayList<String> url_lst = null;
		HashMap<String, Double> score_map = null;
		
		long startTime = System.currentTimeMillis();
		
		url_lst = get_search_urls();
		score_map = get_cosine_score_map(url_lst);
		
		position_scoring ps= new position_scoring();
	    HashMap<String, Double> temp_scoremap = ps.postionScore(resultsetmap, score_map, query_words, query_map);
		if(temp_scoremap!=null)
		{
			score_map = temp_scoremap;
			System.out.println("updated scoring");
		}
		HashMap<String, Double> sorted_score_map = sortByValues(score_map);
		long endtime = System.currentTimeMillis();
		System.out.println("Time for query = " + (endtime - startTime) + "ms");
//					System.out.println("Score map = " + sorted_score_map);
		
		Iterator<String> It = sorted_score_map.keySet().iterator();
		ArrayList<String> URLlist = new ArrayList<String>();
		int num=0;
		while(It.hasNext()){
			URLlist.add(It.next());
			if(num++==50)break;
		}
		return URLlist;
	}
}