import java.io.DataInputStream;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketAddress;

/**
 * Created by Aleksandr_Ivanov1 on 5/15/2017.
 */
public class BillingClient {
    public static final int SERVER_PORT = 7896;
    private String serverName;
    Socket clientSocket;
    DataInputStream dis;
    DataOutputStream dos;

    public BillingClient(String serverName) {
        this.serverName = serverName;
    }

    public static void main(String[] args) {
        BillingClient billingClient = new BillingClient("127.0.0.1");
        try {
            billingClient.startTest();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void startTest() throws IOException {
        connectToServer();
        sendNewCardOperation("Peter", "1");
        sendNewCardOperation("Mark", "2");
        sendNewCardOperation("Inna", "3");
        for (int i = 0; i < 1000; i++) {
            sendAddMoneyOperation("1", 50);
            sendAddMoneyOperation("2", 30);
            sendAddMoneyOperation("3", 20);
        }
        System.out.println("1:" + sendGetCardBalanceOperation("1"));
        System.out.println("2:" + sendGetCardBalanceOperation("2"));
        System.out.println("3:" + sendGetCardBalanceOperation("3"));
        closeConnection();
    }

    private void connectToServer() throws IOException {
        clientSocket = new Socket(serverName, SERVER_PORT);
        dis = new DataInputStream(clientSocket.getInputStream());
        dos = new DataOutputStream(clientSocket.getOutputStream());
    }

    private void sendNewCardOperation(String ownerName, String cardNumber) throws IOException {
        dos.writeInt(MyStoreServer.ADD_NEW_CARD);
        dos.writeUTF(ownerName);
        dos.writeUTF(cardNumber);
    }

    private void sendAddMoneyOperation(String card, double money) throws IOException {
        dos.writeInt(MyStoreServer.ADD_MONEY);
        dos.writeUTF(card);
        dos.writeDouble(money);
    }

    private void sendSubMoneyOperation(String card, double money) throws IOException {
        dos.writeInt(MyStoreServer.SUB_MONEY);
        dos.writeUTF(card);
        dos.writeDouble(money);
    }

    private double sendGetCardBalanceOperation(String card) throws IOException {
        dos.writeInt(MyStoreServer.GET_CARD_BALANCE);
        dos.writeUTF(card);
        return dis.readDouble();
    }

    private void closeConnection() throws IOException {
        dos.writeInt(MyStoreServer.EXIT_CLIENT);
        dis.close();
        dos.close();
        clientSocket.close();
    }
}
