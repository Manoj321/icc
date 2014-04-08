package page_ranking;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

import page_ranking.page_rank;

public class position_scoring {

	 double weight=0.2;
	public  LinkedList<Integer> get_list_from_string(String pos)
	{
		String temp_str;
		temp_str = pos.replaceAll("[^0-9]", " ");
		String[] sar = temp_str.split("\\s+");
		

		LinkedList<Integer> intArray = new LinkedList<Integer>();

		for (int i = 0; i < sar.length; i++)
		{
			if((sar[i].compareTo("")!=0))
			{
				intArray.add(Integer.parseInt(sar[i]));
			}			
		}
		return intArray;
	}
	public  HashMap<String, Double> postionScore(HashMap<String,HashMap<String,Result>> resultsetmap, HashMap<String, Double> score_map, String[] query_words, HashMap<String, Double>query_map){
		if(query_words.length!=2)return null;
		HashMap<String, Result> temp_map = null;
		HashMap<String, Result> temp_map2 = null;
		temp_map = resultsetmap.get(query_words[0]);
		Iterator<String> it = temp_map.keySet().iterator();
		
		System.out.println("map: "+resultsetmap.toString());
		temp_map2 = resultsetmap.get(query_words[1]);
		
		Result pos_res = null;
		int pos_score = 1;
		int scale_factor = 1;
		while(it.hasNext())
		{
			String url = it.next();	
			
			if((url.contains(query_words[0])) && (url.contains(query_words[1])))
			{
				score_map.put(url, (score_map.get(url))+ (score_map.get(url)*weight));
				continue;
			}
			if(url.contains(query_words[0]))
			{
				
				score_map.put(url,(score_map.get(url))+ (score_map.get(url)*weight));
				continue;
			}
			if(url.contains(query_words[1]))
			{
				score_map.put(url, (score_map.get(url)+(score_map.get(url)*weight)));
				continue;
			}
			if(temp_map2.containsKey(url))
			{
				String pos1 = temp_map.get(url).pos;
				String pos2 = temp_map2.get(url).pos;	
				LinkedList<Integer> pos_list1 = get_list_from_string(pos1);
				LinkedList<Integer> pos_list2 = get_list_from_string(pos2);
				
				for(int i=0; i<pos_list1.size(); i++)
				{
					int num = pos_list1.get(i)+ 1;
					if(pos_list2.contains(num))
					{
						pos_score += 1;
					}
				}
				scale_factor = 1;
			
				//System.out.println("pos 1" + pos1 + "pos2" + pos2);
				double score = score_map.get(url);
				score = score * (1+Math.log10(pos_score))*scale_factor;
				
		
				
				score_map.put(url, score);
			}			
		}
		
		it = temp_map2.keySet().iterator();
		while(it.hasNext())
		{
			String url2 = it.next();
			if(url2.contains(query_words[1]))
			{
				score_map.put(url2, (score_map.get(url2))+ (score_map.get(url2)*weight));
			}
			if(url2.contains(query_words[1]) && url2.contains(query_words[0]))
			{
				score_map.put(url2, (score_map.get(url2))+ (score_map.get(url2)*weight));
			}
		}
		return score_map;
	}
}