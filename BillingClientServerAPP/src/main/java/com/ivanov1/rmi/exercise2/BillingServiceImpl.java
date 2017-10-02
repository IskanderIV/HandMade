package com.ivanov1.rmi.exercise2;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Date;
import java.util.Hashtable;

/**
 * Created by @author Aleksandr_Ivanov1 on 5/14/2017.
 *
 * @
 */
public class BillingServiceImpl extends UnicastRemoteObject implements BillingService {
    private Hashtable cardMemory;
    private static String configFileName = "config.properties";

    public static void main(String[] args) throws Exception {
        System.out.println("Start server..");
        BillingServiceImpl billingServiceImpl = new BillingServiceImpl();
        String serviceName = "rmi://localhost/BillingService";
        Registry stReg = LocateRegistry.createRegistry(Registry.REGISTRY_PORT);
        System.out.println("Launching of RMI registry..");
        Naming.rebind(serviceName, billingServiceImpl);
        System.out.println("Ready..");
    }

    // расширяем конструктор UnicastRemoteObject
    public BillingServiceImpl() throws RemoteException {
        super();
        cardMemory = new Hashtable();
    }

    @Override
    public void addNewCard(Card card) throws RemoteException {
        cardMemory.put(card.getCardNumber(), card);
        System.out.println("Card " + card.getCardNumber() + " created for " + card.getPerson());
    }

    @Override
    public void processOperations(CardOperation[] operations) throws RemoteException {
        for (int i = 0; i < operations.length; i++) {
            Card c = (Card) cardMemory.get(operations[i].getCard());
            if (c == null) throw new NotExistsCardOperation();
            c.setBalance(c.getBalance() + operations[i].getAmount());
            cardMemory.put(operations[i].getCard(), c);
        }
    }

    @Override
    public Card getCard(String card) throws RemoteException {
        Card c = (Card) cardMemory.get(card);
        return c;
    }
}
