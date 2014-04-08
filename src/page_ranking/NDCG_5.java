package page_ranking;

import java.util.LinkedList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

class NDCG_5 {

	public static HashMap<String, LinkedList<String>> create_google_map()
	{
	 	HashMap<String, LinkedList<String>> google_map = new HashMap<String, LinkedList<String>>();
	 	
	 	LinkedList<String> list1 = new LinkedList<String>();
	 	list1.add("http://mondego.ics.uci.edu/");
	 	list1.add("http://sdcl.ics.uci.edu/mondego-group/");
	 	list1.add("http://www.ics.uci.edu/~lopes/");
	 	list1.add("http://www.ics.uci.edu/~lopes/datasets/");	 	
	 	list1.add("http://www.ics.uci.edu/~lopes/datasets/sourcerer-maven-aug12.html");
	 	
	 	LinkedList<String> list2= new LinkedList<String>();
	 	list2.add("http://archive.ics.uci.edu/ml/");
	 	list2.add("http://archive.ics.uci.edu/ml/datasets.html");
	 	list2.add("http://cml.ics.uci.edu/");
	 	list2.add("http://cml.ics.uci.edu/?page=events&subPage=aiml");
	 	list2.add("http://mlearn.ics.uci.edu/MLRepository.html");
	 	
	 	LinkedList<String> list3 = new LinkedList<String>();
	 	list3.add("http://www.ics.uci.edu/prospective/en/degrees/software-engineering/");
	 	list3.add("http://www.ics.uci.edu/grad/degrees/degree_se.php");
	 	list3.add("http://se.ics.uci.edu/");
	 	list3.add("http://www.ics.uci.edu/~djr/DebraJRichardson/SE4S.html");
	 	list3.add("http://www.ics.uci.edu/prospective/en/degrees/computer-science-engineering/");
	 	
	 	LinkedList<String> list4 = new LinkedList<String>();
	 	list4.add("http://www.ics.uci.edu/computing/linux/security.php");
	 	list4.add("http://www.ics.uci.edu/faculty/area/area_security.php");
	 	list4.add("http://www.ics.uci.edu/community/news/spotlight/spotlight_tsudik_a_whole_new_class.php");
	 	list4.add("http://sconce.ics.uci.edu/");
	 	list4.add("http://www.ics.uci.edu/~goodrich/teach/ics8/");
	 	
	 	LinkedList<String> list5 = new LinkedList<String>();
	 	list5.add("http://www.ics.uci.edu/about/search/search_sao.php");
	 	list5.add("http://www.ics.uci.edu/prospective/en/contact/student-affairs/");
	 	list5.add("http://www.ics.uci.edu/ugrad/sao/");
	 	list5.add("http://www.ics.uci.edu/grad/sao/");
	 	list5.add("http://www.ics.uci.edu/about/visit/");
	 	
	 	LinkedList<String> list6 = new LinkedList<String>();
	 	list6.add("http://www.ics.uci.edu/grad/courses/index.php");
	 	list6.add("http://www.ics.uci.edu/grad/");
	 	list6.add("http://www.ics.uci.edu/grad/policies/"); 	
	 	list6.add("http://www.ics.uci.edu/grad/courses/listing.php?year=2013&level=ALL&department=INF&program=0CSE");
	 	list6.add("http://www.cs.uci.edu/grad/");
	 	
	 	LinkedList<String> list7 = new LinkedList<String>();
	 	list7.add("http://www.ics.uci.edu/~lopes/");
	 	list7.add("http://www.ics.uci.edu/~lopes/publications.html");
	 	list7.add("http://luci.ics.uci.edu/blog/?tag=crista-lopes");
	 	list7.add("http://www.ics.uci.edu/~tdebeauv/");
	 	list7.add("http://mondego.ics.uci.edu/");
	 	
	 	LinkedList<String> list8 = new LinkedList<String>();
	 	list8.add("http://www.ics.uci.edu/~fielding/pubs/dissertation/rest_arch_style.htm");
	 	list8.add("http://www.ics.uci.edu/~fielding/pubs/dissertation/top.htm");
	 	list8.add("http://www.ics.uci.edu/~fielding/pubs/dissertation/evaluation.htm");
	 	list8.add("http://www.ics.uci.edu/~fielding/pubs/dissertation/introduction.htm");
	 	list8.add("http://www.ics.uci.edu/~fielding/pubs/dissertation/conclusions.htm");
	 	
	 	LinkedList<String> list9 = new LinkedList<String>();
	 	list9.add("http://www.ics.uci.edu/prospective/en/degrees/computer-game-science/");
	 	list9.add("http://www.ics.uci.edu/ugrad/degrees/degree_cgs.php");
	 	list9.add("http://cgvw.ics.uci.edu/");
	 	list9.add("http://www.ics.uci.edu/~eppstein/gina/vidgames.html");
	 	list9.add("http://frost.ics.uci.edu/cs113/");
	 	
	 	LinkedList<String> list10 = new LinkedList<String>();
	 	list10.add("http://www.ics.uci.edu/~lopes/teaching/cs221W12/");
	 	list10.add("http://www.ics.uci.edu/~djp3/classes/2010_01_CS221/");
	 	list10.add("http://www.ics.uci.edu/~lopes/");
	 	list10.add("http://www.ics.uci.edu/~djp3/classes/2014_01_INF141/calendar.html");
	 	list10.add("http://www.ics.uci.edu/~djp3/classes/2014_01_INF141/structure.html");
	 	
	 	google_map.put("mondego", list1);
	 	google_map.put("machine learning", list2);
	 	google_map.put("software engineering", list3);
	 	google_map.put("security", list4);
	 	google_map.put("student affairs", list5);
	 	google_map.put("graduate course", list6);
	 	google_map.put("crista lopes", list7);
	 	google_map.put("rest", list8);
	 	google_map.put("computer game", list9);
	 	google_map.put("inform retriev", list10);
	 	
	 	return google_map;
	 	
	}
	
	public static double calculate_ndcg(String query, LinkedList<String> url_list)
	{
		HashMap<String,LinkedList<String>> google_resultmap=create_google_map();
		
		Iterator<String> It = url_list.iterator();
	    LinkedList<String> google_querymap = google_resultmap.get(query);
	    if(google_querymap==null)return 1.0;
	    String url;
	    int count = 0, indx;
	    double ndcg = 0;
	    while(It.hasNext() && count <5)
	    {
	    	url = It.next();
	    	count++;	
	    	if(google_querymap.contains(url))
	    	{
	    		
	    		indx = google_querymap.indexOf(url);
	    		System.out.println("indx " + indx);
	    		ndcg += (Math.pow(2.0, 5-(indx)) - 1)/((Math.log10(count + 1)/Math.log10(2)));  		
	    	}
	    }
		
	    ndcg /= (double)45.64282878502658;
		return ndcg;
	}
	
}

