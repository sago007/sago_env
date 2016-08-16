package sago.mongodb_test;

import java.security.GeneralSecurityException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TimeZone;

import org.apache.commons.cli.*;

public class mongodb_testMain {
    public final static void main(String[] args) {
        Options options = new Options();
        options.addOption("h", "help", false, "Show this help message");
        options.addOption("", "add-json", true, "Add a json message to the database");
        options.addOption("a", "appName", true, "The appName used as the key");
        CommandLineParser parser = new DefaultParser();
        String theMessage = "";
        String appName = "myApp";
        try {
            CommandLine cmd = parser.parse( options, args);
            if (cmd.hasOption('h')) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp( "mongodb_test", options );
                System.exit(0);
            }
            if (cmd.hasOption("add-json")) {
            	String value = cmd.getOptionValue( "add-json" );
            	if (value != null) {
            		theMessage = value;
            	}
            }
            if (cmd.hasOption("appName")) {
            	String value = cmd.getOptionValue( "appName" );
            	if (value != null) {
            		appName = value;
            	}
            }
        } catch (ParseException e) {
            System.err.println("Program parameter error: "+e);
            System.exit(1);
        }
        System.out.println("Basic program! Message: "+theMessage);
        MongoDbStore mongo = new MongoDbStore();
        Map<String, String> jo = new HashMap<String,String>();
        if (theMessage.length() > 0) {
        	jo.put("msg", theMessage);
        	jo.put("ts", getNow());
        	jo.put("appName", appName);
        	mongo.StoreData(jo);
        }
        
        List<Map<String,String>> l = mongo.getEntryList(appName);
        System.out.println("Values stored:");
        for (Map<String,String> s : l) {
        	System.out.println("--- START ---");
        	for (String k :s.keySet()) {
        		System.out.println(k+": "+s.get(k));
        	}
        	System.out.println("---- END ----");
        }
        
        System.out.println("unique stored:");
        Map<String,Map<String,String>> s = mongo.GetLastEntry();
        for (Entry<String,Map<String,String>> e : s.entrySet()) {
        	System.out.println("--- START ---");
        	System.out.println("* App: "+e.getKey());
        	for (Entry<String,String> k :e.getValue().entrySet()) {
        		System.out.println(k.getKey()+": "+k.getValue());
        	}
        	System.out.println("---- END ----");
        }
    }
    
    public static String getNow() {
    	TimeZone tz = TimeZone.getTimeZone("UTC");
    	DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'"); // Quoted "Z" to indicate UTC, no timezone offset
    	df.setTimeZone(tz);
    	String nowAsISO = df.format(new Date());
    	return nowAsISO;
    }
}
