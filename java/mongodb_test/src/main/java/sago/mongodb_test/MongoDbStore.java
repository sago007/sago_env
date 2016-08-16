package sago.mongodb_test;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.Map.Entry;
import java.util.Set;

import org.bson.Document;

import com.mongodb.BasicDBObject;
import com.mongodb.MongoClient;
import com.mongodb.client.FindIterable;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;

public class MongoDbStore {
	MongoClient mongo;
    MongoDatabase db;
    String storeName = "myStore";
	
	public MongoDbStore() {
		String host = "localhost";
    	int port = 27017;
    	String dbName = "SagoTest";
        try {
            mongo = new MongoClient(host, port);
            db = mongo.getDatabase(dbName);
        } catch (Exception ex) {
        	String errMsg = "Failed to open db \""+dbName+"\" on \""+host+":"+port+"\"";
        	System.err.println(errMsg);
            throw new RuntimeException(errMsg);
        }
	} 
	
	
	
	void StoreData(Map<String,String> mapData) {
		MongoCollection<Document> collection = db.getCollection(storeName);
		Document d = new Document();
		d.putAll(mapData);
		collection.insertOne(d);
	}
	
	Map<String,String> Document2Map(Document d) {
		Set<Entry<String, Object>> s = d.entrySet();
		Map<String,String> m = new TreeMap<String, String>();
		for (Entry<String, Object> e : s) {
			m.put(e.getKey(), e.getValue().toString());
		}
		return m;
	}
	
	List<Map<String, String>> getJsonList(String appName) {
		List<Map<String,String>> res = new ArrayList<Map<String,String>>();
		BasicDBObject query = new BasicDBObject();
		query.append("appName",  new BasicDBObject("$eq", appName));
		MongoCollection<Document> collection = db.getCollection(storeName);
		FindIterable<Document> cursor = collection.find(query);
		for (Document d : cursor) {
			Map<String, String> m = Document2Map(d);
			res.add(m);
		}
		return res;
	}
	
	Map<String,Map<String,String>> GetLastEntry() {
		Map<String,Map<String,String>> res = new TreeMap<String,Map<String,String>>();
		MongoCollection<Document> collection = db.getCollection(storeName);
		MongoCursor<String> c = collection.distinct("appName", String.class).iterator();
		while (c.hasNext()) {
			String s = c.next();
			BasicDBObject query = new BasicDBObject();
			query.append("appName",  new BasicDBObject("$eq", s));
			query.append("sort", new BasicDBObject("ts", -1));
			FindIterable<Document> cursor = collection.find(query);
			Document d = cursor.first();
			if (d != null) {
				Map<String, String> value = Document2Map(d);
				res.put(s, value);
			}
		}
		return res;
	}
}
