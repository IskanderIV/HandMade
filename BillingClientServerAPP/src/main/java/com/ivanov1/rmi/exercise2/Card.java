package com.ivanov1.rmi.exercise2;

import java.io.Serializable;
import java.util.Date;

/**
 * Created by Aleksandr_Ivanov1 on 5/17/2017.
 */
public class Card implements Serializable {
    private String person;
    private Date createDate;
    private String cardNumber;
    private double balance;

    public Card(String person, Date createDate, String cardNumber, double balance) {
        this.person = person;
        this.createDate = createDate;
        this.cardNumber = cardNumber;
        this.balance = balance;
    }

    public String getPerson() {
        return person;
    }

    public void setPerson(String person) {
        this.person = person;
    }

    public Date getCreateDate() {
        return createDate;
    }

    public void setCreateDate(Date createDate) {
        this.createDate = createDate;
    }

    public String getCardNumber() {
        return cardNumber;
    }

    public void setCardNumber(String cardNumber) {
        this.cardNumber = cardNumber;
    }

    public double getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }

    @Override
    public String toString() {
        return "Card{" +
                "person='" + person + '\'' +
                ", createDate=" + createDate +
                ", cardNumber='" + cardNumber + '\'' +
                ", balance=" + balance +
                '}';
    }
}
