import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Hashtable;
import java.util.Properties;

/**
 * Created by @author Aleksandr_Ivanov1 on 5/14/2017.
 *
 * @
 */
public class MyStoreServer extends Thread {
    private ServerSocket serverSocket;
    private Hashtable cardMemory;
    private static String configFileName = "config.properties";
    public static final int ADD_NEW_CARD = 1; //PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.add_new_card");
    public static final int ADD_MONEY = 2; //PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.add_money");
    public static final int SUB_MONEY = 3; //PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.sub_money");
    public static final int GET_CARD_BALANCE = 4; //PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.get_card_balance");
    public static final int EXIT_CLIENT = 5; //PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.exit_client");
    public static final int SERVER_PORT = 7896; //Integer.parseInt(PropertyReader.getInfo(configFileName).getProperty("com.epam.ivanov.server.port"));


    public static void main(String[] args) {
        MyStoreServer myStoreServer = new MyStoreServer();
        myStoreServer.start();
    }

    public MyStoreServer() {
        cardMemory = new Hashtable();
    }

    @Override
    public void run() {
        try {
            serverSocket = new ServerSocket(SERVER_PORT);
            System.out.println("Server started");
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Server started");
                BillingClientService bcs = new BillingClientService(this,
                        new DataInputStream(clientSocket.getInputStream()),
                        new DataOutputStream(clientSocket.getOutputStream()));
                bcs.start();
            }
        } catch (IOException ioEx) {
            System.out.println(ioEx.getMessage());
        }
    }

    public void addNewCard(String PersonName, String card) {
        cardMemory.put(card, new Double(0.0));
    }

    public void addMoney(String card, double money) {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            cardMemory.put(card, new Double(moneyAccount.doubleValue() + money));
        }
    }

    public void subMoney(String card, double money) {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            cardMemory.put(card, new Double(moneyAccount.doubleValue() - money));
        }
    }

    public double getCardBalance(String card) {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            return moneyAccount.doubleValue();
        }
        return 0.0;
    }
}
