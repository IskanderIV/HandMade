package com.ivanov1.rmi.exercise2;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Created by Aleksandr_Ivanov1 on 5/16/2017.
 */
public interface BillingService extends Remote {
    public void addNewCard(Card card) throws RemoteException;

    public void processOperations(CardOperation[] operations) throws RemoteException;

    public Card getCard(String card) throws RemoteException;
}
