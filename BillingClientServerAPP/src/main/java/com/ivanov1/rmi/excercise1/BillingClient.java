package com.ivanov1.rmi.excercise1;

import com.ivanov1.rmi.exercise2.Card;

import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * Created by Aleksandr_Ivanov1 on 5/15/2017.
 */
public class BillingClient {
    public static final String SERVER_NAME = "10.11.17.158";
    public static final String PORT = "8888";
    private String serverName;

    public BillingClient() {
        this.serverName = SERVER_NAME;
    }

    public static void main(String[] args) throws Exception {
        String objectName = "rmi://" + SERVER_NAME + ":" + PORT + "/BillingService";
        BillingClient billingClient = new BillingClient();
        System.out.println("Starting..\n");
        // соединение с реестром RMI и получение удаленной ссылки на удаленный объект
//        Registry reg = LocateRegistry.getRegistry(Server.PORT);
        BillingService bs = (BillingService) Naming.lookup(objectName);
        System.out.println("done\n");

        for (int i = 0; i < 10000; i++) {
            try {
                bs.addMoney("1", 1);
            } catch (RemoteException rEx) {
                bs.addNewCard("Piter", "1");
            }

            try {
                bs.addMoney("2", 2);
            } catch (RemoteException rEx) {
                bs.addNewCard("Stefan", "2");
            }

            try {
                bs.addMoney("3", 3);
            } catch (RemoteException rEx) {
                bs.addNewCard("Nataly", "3");
            }
        }

        System.out.println("1: " + bs.getCardBalance("1"));
        System.out.println("2: " + bs.getCardBalance("2"));
        System.out.println("3: " + bs.getCardBalance("3"));
    }
}
