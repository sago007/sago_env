package sago.hello_jersey.web.types;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class Hello {
	private String message = "Hello World";
	private String receiver = "World";
	
	public Hello() {};
	
	public String getReceiver() {
		return receiver;
	}
	public void setReceiver(String receiver) {
		this.receiver = receiver;
	}
	
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
}
