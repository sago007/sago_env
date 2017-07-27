package sago.baseprogram;

import org.apache.commons.cli.*;

public class baseprogramMain {
    public final static void main(String[] args) {
        Options options = new Options();
        options.addOption("h", "help", false, "Show this help message");
        CommandLineParser parser = new DefaultParser();
        try {
            CommandLine cmd = parser.parse( options, args);
            if (cmd.hasOption('h')) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp( "baseprogram", options );
                System.exit(0);
            }
        } catch (ParseException e) {
            System.err.println("Program parameter error: "+e);
            System.exit(1);
        }
        System.out.println("Basic program!");
    }
}
