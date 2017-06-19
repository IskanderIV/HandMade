package com.ivanov1.rmi.excercise1;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Created by Aleksandr_Ivanov1 on 5/16/2017.
 */
public interface BillingService extends Remote {
    public void addNewCard(String personName, String card) throws RemoteException;

    public void addMoney(String card, double money) throws RemoteException;

    public void subMoney(String card, double money) throws RemoteException;

    public double getCardBalance(String card) throws RemoteException;
}
