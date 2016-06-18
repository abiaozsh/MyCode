package webServer;

import java.util.HashMap;
import java.util.Random;
import java.util.TimerTask;

public final class Session {

    private String ID;

    TimerTask cleanTask;

    Server server;

    Session(Server server) {
        this.server = server;
        synchronized (server) {
            while (true) {
                ID = Session.getNewSessionId();
                if (server.getSession(ID) == null) {
                    break;
                }
            }
            server.setSession(ID, this);
        }
        synchronized (server.sessionCleaner) {
            cleanTask = new SessionCleaner(this, server);
            server.sessionCleaner.schedule(cleanTask, server.currentConfig.getSessionExpires() * 1000);
            Server.debug("new Session: " + ID + " at " + Util.getTime());
        }
    }

    void touch() {
        synchronized (server.sessionCleaner) {
            cleanTask.cancel();
            server.sessionCleaner.purge();
            cleanTask = new SessionCleaner(this, server);
            server.sessionCleaner.schedule(cleanTask, server.currentConfig.getSessionExpires() * 1000);
            Server.debug("touch Session: " + ID + " at " + Util.getTime());
        }
    }

    private HashMap<String, Object> data = new HashMap<String, Object>();

    public Object get(String id) {
        return data.get(id);
    }

    public void set(String id, Object obj) {
        data.put(id, obj);
    }

    public void remove(String id) {
        data.remove(id);
    }

    private HashMap<String, Object> privateData = new HashMap<String, Object>();

    Object getPrivate(String id) {
        return privateData.get(id);
    }

    void setPrivate(String id, Object obj) {
        privateData.put(id, obj);
    }

    void removePrivate(String id) {
        privateData.remove(id);
    }

    String getId() {
        return ID;
    }

    static String getNewSessionId() {
        String sessionId;
        Random rnd = new Random();
        byte[] b = new byte[16];
        StringBuffer sb = new StringBuffer();
        rnd.nextBytes(b);
        for (int i = 0; i < 16; i++) {
            int v = 0xFF & b[i];
            if (v <= 0x0f) {
                sb.append("0");
            }
            sb.append(Integer.toHexString(v));
        }
        sessionId = sb.toString();
        return sessionId;
    }
}

final class SessionCleaner extends TimerTask {
    int sessionExpires;

    Session session;
    Server server;

    SessionCleaner(Session session, Server server) {
        this.session = session;
        this.server = server;
    }

    @Override
    public void run() {
        synchronized (server.sessionPooling) {
            server.sessionPooling.remove(session.getId());
            Server.debug("close Session: " + session.getId() + " at " + Util.getTime());
        }
    }
}
