package webServer;
public class Main {

    public static void main(String[] Args) throws Exception {
        Server sv = new Server();
        sv.init("webServer.log");
        sv.readConfigFromFile("config.xml");
        sv.applyConfig();
        sv.startService();
        System.out.println("Server started.");
    }
}
