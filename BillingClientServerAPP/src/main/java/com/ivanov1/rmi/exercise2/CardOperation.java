package com.ivanov1.rmi.exercise2;

import java.io.Serializable;
import java.util.Date;

/**
 * Created by Aleksandr_Ivanov1 on 5/17/2017.
 */
public class CardOperation implements Serializable {
    private String card;
    private Date operationDate;
    private double amount;

    public CardOperation(String card, double amount, Date operationDate) {
        this.card = card;
        this.operationDate = operationDate;
        this.amount = amount;
    }

    public String getCard() {
        return card;
    }

    public void setCard(String card) {
        this.card = card;
    }

    public Date getOperationDate() {
        return operationDate;
    }

    public void setOperationDate(Date operationDate) {
        this.operationDate = operationDate;
    }

    public double getAmount() {
        return amount;
    }

    public void setAmount(double amount) {
        this.amount = amount;
    }

    @Override
    public String toString() {
        return "CardOperation{" +
                "card='" + card + '\'' +
                ", operationDate=" + operationDate +
                ", amount=" + amount +
                '}';
    }
}
