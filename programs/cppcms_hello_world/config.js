{
   "application" : {  
      // user specific data  
      "connection_string" : "mysql:database=hello",  
   },  
    "service" : {
        "api" : "http",
        "port" : 8080
    },
    "http" : {
        "script_names" :  ["/hello"]
    },
    "file_server" : {
        "enable": false,
	"document_root" : ".",
	"alias" : [  
    		{ "url" : "/files/media" , "path" : "./media" },  
		{ "url" : "/files/static/" ,"path" : "./static_media"},
  	],  
    }
}
