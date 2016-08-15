package sago.mongodb_test;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.bson.Document;

import com.eclipsesource.json.JsonObject;
import com.eclipsesource.json.JsonObject.Member;
import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;

public class MongoDbThing {
	MongoClient mongo;
    MongoDatabase db;
	
	public MongoDbThing() {
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
	
	void StoreJson(String appName, JsonObject json) {
		MongoCollection<Document> collection = db.getCollection("myStore");
		Document d = new Document();
		Map<String, String> m = new HashMap<String, String>(); 
		for (Member member : json) {
			if (member.getValue() != null) {
				m.put(member.getName(), member.getValue().toString());
			}
		}
		d.putAll(m);
		collection.insertOne(d);
		
	}
	
	List<JsonObject> getJsonList(String appName) {
		throw new RuntimeException("getJsonList not implemented");
	}
}
