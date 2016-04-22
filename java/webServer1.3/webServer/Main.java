package webServer;
public class Main {

    public static void main(String[] Args) throws Exception {
        Server.serverInstance = new Server();
        Server.serverInstance.init("config.xml");
        Log.initLog(Server.serverInstance.currentConfig.logFile);
        Server.serverInstance.applyConfig();
        Server.serverInstance.startService();
        System.out.println("Server started.");
    }
}
