package com.ivanov1.rmi.excercise1;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
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
        String serviceName = "rmi://localhost:5050/BillingService";
        Registry stReg = LocateRegistry.createRegistry(5050);
        System.out.println("Launching of RMI registry..");
        Naming.rebind(serviceName, billingServiceImpl);
        System.out.println("Good rebinding..");
    }

    // расширяем конструктор UnicastRemoteObject
    public BillingServiceImpl() throws RemoteException {
        super();
        cardMemory = new Hashtable();
    }

    public void addNewCard(String personName, String card) throws RemoteException {
        cardMemory.put(card, new Double(0.0));
        System.out.println("Card " + card + " created for " + personName);

    }

    public void addMoney(String card, double money) throws RemoteException {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            cardMemory.put(card, new Double(moneyAccount.doubleValue() + money));
        } else {
            throw new NotExistsCardOperation();
        }
    }

    public void subMoney(String card, double money) throws RemoteException {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            cardMemory.put(card, new Double(moneyAccount.doubleValue() - money));
        } else {
            throw new NotExistsCardOperation();
        }
    }

    public double getCardBalance(String card) throws RemoteException {
        Double moneyAccount = (Double) cardMemory.get(card);
        if (moneyAccount != null) {
            return moneyAccount.doubleValue();
        } else {
            throw new NotExistsCardOperation();
        }
    }


}
