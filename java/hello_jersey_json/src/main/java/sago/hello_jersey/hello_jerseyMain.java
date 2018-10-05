package sago.hello_jersey;

import org.apache.commons.cli.*;

import org.glassfish.grizzly.http.server.HttpServer;
import org.glassfish.jersey.grizzly2.httpserver.GrizzlyHttpServerFactory;
import org.glassfish.jersey.jackson.JacksonFeature;
import org.glassfish.jersey.server.ResourceConfig;

import java.net.URI;

public class hello_jerseyMain {

	// Base URI the Grizzly HTTP server will listen on
	public static final String BASE_URI = "http://localhost:8080/hello_jersey/";

	/**
	 * Starts Grizzly HTTP server exposing JAX-RS resources defined in this
	 * application.
	 * 
	 * @return Grizzly HTTP server.
	 */
	public static HttpServer startServer() {
		// create a resource config that scans for JAX-RS resources and providers
		// in com.example package
		final ResourceConfig rc = new ResourceConfig().packages("sago.hello_jersey.web");

		//Add json support
		rc.register(JacksonFeature.class);
		
		// create and start a new instance of grizzly http server
		// exposing the Jersey application at BASE_URI
		return GrizzlyHttpServerFactory.createHttpServer(URI.create(BASE_URI), rc);
	}

	public final static void main(String[] args) {
		Options options = new Options();
		options.addOption("h", "help", false, "Show this help message");
		CommandLineParser parser = new DefaultParser();
		try {
			CommandLine cmd = parser.parse(options, args);
			if (cmd.hasOption('h')) {
				HelpFormatter formatter = new HelpFormatter();
				formatter.printHelp("hello_jersey", options);
				System.exit(0);
			}
		} catch (ParseException e) {
			System.err.println("Program parameter error: " + e);
			System.exit(1);
		}
		System.out.println("Basic program!");
		final HttpServer server = startServer();
		System.out
				.println(String.format("Jersey app started with WADL available at " + "%sapplication.wadl", BASE_URI));
		// System.in.read();
		// server.stop();
	}
}
