package sago.hello_jersey.web;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.MediaType;

import sago.hello_jersey.*;

import org.glassfish.grizzly.http.server.HttpServer;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class MyResourceTest {

	private HttpServer server;
	private WebTarget target;

	@Before
	public void setUp() throws Exception {
		// start the server
		server = hello_jerseyMain.startServer();
		// create the client
		Client c = ClientBuilder.newClient();

		//Json support
		//c.configuration().enable(new org.glassfish.jersey.media.json.JsonJaxbFeature());

		target = c.target(hello_jerseyMain.BASE_URI);
	}

	@After
	public void tearDown() {
		server.shutdownNow();
	}

	/**
	 * Test to see that the message "Got it!" is sent in the response.
	 */
	@Test
	public void testGetIt() {
		String responseMsg = target.path("myresource").request().accept(MediaType.TEXT_PLAIN).get(String.class);
		assertEquals("Hello World!", responseMsg);
	}
}
