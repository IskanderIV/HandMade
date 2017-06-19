package com.ivanov1.rmi.exercise2;

import java.rmi.Naming;
import java.util.Date;

/**
 * Created by Aleksandr_Ivanov1 on 5/15/2017.
 */
public class BillingClient {
    public static final String SERVER_NAME = "127.0.0.1";
    public static final String PORT = "1099";
    private String serverName;

    public BillingClient() {
        this.serverName = SERVER_NAME;
    }

    public static void main(String[] args) throws Exception {
        String objectName = "rmi://" + SERVER_NAME + ":" + PORT + "/BillingService";
        BillingClient billingClient = new BillingClient();
        System.out.println("Starting..\n");
        BillingService bs = (BillingService) Naming.lookup(objectName);
        System.out.println("done\n");

        Card card;
        card = bs.getCard("1");
        if (card == null) {
            card = new Card("Peter", new Date(), "1", 0.0);
            bs.addNewCard(card);
        }
        card = bs.getCard("2");
        if (card == null) {
            card = new Card("Stefan", new Date(), "2", 0.0);
            bs.addNewCard(card);
        }
        card = bs.getCard("3");
        if (card == null) {
            card = new Card("Nataly", new Date(), "3", 0.0);
            bs.addNewCard(card);
        }

        System.err.println("begin..\n");
        int counter = 30000;
        CardOperation[] co = new CardOperation[counter];
        for (int i = 0; i < counter; i++) {
            switch (i % 3) {
                case 0:
                    co[i] = new CardOperation("1", 1, new Date());
                    break;
                case 1:
                    co[i] = new CardOperation("2", 2, new Date());
                    break;
                case 2:
                    co[i] = new CardOperation("3", 3, new Date());
                    break;
            }
        }

        System.out.println(bs.getCard("1"));
        System.out.println(bs.getCard("2"));
        System.out.println(bs.getCard("3"));

        bs.processOperations(co);

        System.out.println(bs.getCard("1"));
        System.out.println(bs.getCard("2"));
        System.out.println(bs.getCard("3"));
    }
}
