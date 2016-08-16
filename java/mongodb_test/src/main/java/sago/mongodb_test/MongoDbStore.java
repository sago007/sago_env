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
	
	/**
	 * Helper function to die if the user has given a null parameter for a non-nullable argument
	 * @param o The object to check for null
	 * @param errMsg The error message in the RuntimeException thrown
	 */
	private void ThrowIfNull(Object o, String errMsg) {
		if (o == null) {
			throw new RuntimeException(errMsg);
		}
	}
	
	/**
	 * Stores data. The map must have a key named "appName".
	 * @param mapData The map with the values for the data. Must not be null. Must have a key named "appName"
	 */
	public void StoreData(Map<String,String> mapData) {
		ThrowIfNull(mapData, "MongoDbStore::StoreData called with mapData == null");
		String appName = mapData.get("appName");
		ThrowIfNull(appName, "MongoDbStore::StoreData error: mapData doew not have a key named \"appName\"");
		MongoCollection<Document> collection = db.getCollection(storeName);
		Document d = new Document();
		d.putAll(mapData);
		collection.insertOne(d);
	}
	
	private Map<String,String> Document2Map(Document d) {
		Set<Entry<String, Object>> s = d.entrySet();
		Map<String,String> m = new TreeMap<String, String>();
		for (Entry<String, Object> e : s) {
			m.put(e.getKey(), e.getValue().toString());
		}
		return m;
	}
	
	/**
	 * Retrieves a list of all entries that belongs to a given appName
	 * @param appName Name of the app to get entries for. Must not be null.
	 * @return List with all the entries. Never null.
	 */
	public List<Map<String, String>> getEntryList(String appName) {
		ThrowIfNull(appName, "MongoDbStore::getEntryList called with appName == null");
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
	
	/**
	 * Retrieves all unique appNames. And the latest entry.
	 * @return A map with the appName as key and the last entry as value. Never null.
	 */
	public Map<String,Map<String,String>> GetLastEntry() {
		Map<String,Map<String,String>> res = new TreeMap<String,Map<String,String>>();
		MongoCollection<Document> collection = db.getCollection(storeName);
		MongoCursor<String> c = collection.distinct("appName", String.class).iterator();
		while (c.hasNext()) {
			String s = c.next();
			BasicDBObject query = new BasicDBObject();
			query.append("appName",  new BasicDBObject("$eq", s));
			FindIterable<Document> cursor = collection.find(query);
			cursor.sort(new BasicDBObject("ts", -1));
			Document d = cursor.first();
			if (d != null) {
				Map<String, String> value = Document2Map(d);
				res.put(s, value);
			}
		}
		return res;
	}
}
