package sago.hello_jersey.web;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import sago.hello_jersey.web.types.*;

/**
 * Root resource (exposed at "myresource" path)
 */
@Path("myresource")
public class MyResource {

	/**
	 * Method handling HTTP GET requests. The returned object will be sent to the
	 * client as "text/plain" media type.
	 *
	 * @return String that will be returned as a text/plain response.
	 */
	@GET
	@Produces(MediaType.TEXT_PLAIN)
	public String getIt() {
		return "Hello World!";
	}

	// This method is called if JSON is requested
	@GET
	@Produces(MediaType.APPLICATION_JSON)
	public Hello getJSON() {
		Hello hello = new Hello();
		return hello;
	}

}
