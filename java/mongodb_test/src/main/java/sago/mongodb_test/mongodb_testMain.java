package sago.mongodb_test;

import org.apache.commons.cli.*;

import com.eclipsesource.json.JsonObject;

public class mongodb_testMain {
    public final static void main(String[] args) {
        Options options = new Options();
        options.addOption("h", "help", false, "Show this help message");
        options.addOption("", "add-json", true, "Add a json message to the database");
        CommandLineParser parser = new DefaultParser();
        String theMessage = "";
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
        } catch (ParseException e) {
            System.err.println("Program parameter error: "+e);
            System.exit(1);
        }
        System.out.println("Basic program! Message: "+theMessage);
        MongoDbThing mongo = new MongoDbThing();
        JsonObject jo = new JsonObject();
        if (theMessage.length() > 0) {
        	jo.add("msg", theMessage);
        	mongo.StoreJson("Somename", jo);
        }
    }
}
