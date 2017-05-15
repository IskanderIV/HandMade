import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/**
 * Created by Aleksandr_Ivanov1 on 5/15/2017.
 *
 */
public class BillingClientService extends Thread {
    DataInputStream inputData;
    DataOutputStream outputData;
    MyStoreServer myStoreServer;

    public BillingClientService(MyStoreServer myStoreServer, DataInputStream inputData, DataOutputStream outputData) {
        this.inputData = inputData;
        this.outputData = outputData;
        this.myStoreServer = myStoreServer;
    }

    public void run() {
        boolean closeConnection = false;
        while (!closeConnection) {
            try {
                int command = inputData.readInt();
                switch (command) {
                    case MyStoreServer.ADD_NEW_CARD: {
                        addNewCard();
                        break;
                    }
                    case MyStoreServer.ADD_MONEY: {
                        addMoney();
                        break;
                    }
                    case MyStoreServer.SUB_MONEY: {
                        subMoney();
                        break;
                    }
                    case MyStoreServer.GET_CARD_BALANCE: {
                        getCardBalance();
                        break;
                    }
                    case MyStoreServer.EXIT_CLIENT: {
                        System.out.println("Connection closed");
                        closeConnection = true;
                        break;
                    }
                    default:
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void addNewCard() throws IOException {
        String personName = inputData.readUTF();
        String card = inputData.readUTF();
        myStoreServer.addNewCard(personName, card);
        System.out.println("Adding new card " + card + " from " + personName);
    }

    private void addMoney() throws IOException {
        String card = inputData.readUTF();
        double money = inputData.readDouble();
        myStoreServer.addMoney(card, money);
        System.out.println("Adding " + money +" money to card " + card);
    }

    private void subMoney() throws IOException {
        String card = inputData.readUTF();
        double money = inputData.readDouble();
        myStoreServer.subMoney(card, money);
        System.out.println("Subdividing " + money +" money from card " + card);
    }

    private void getCardBalance() throws IOException {
        double balance;
        String card = inputData.readUTF();
        balance = myStoreServer.getCardBalance(card);
        outputData.writeDouble(balance);
        System.out.println("Getting card " + card + " balance");
    }
}
