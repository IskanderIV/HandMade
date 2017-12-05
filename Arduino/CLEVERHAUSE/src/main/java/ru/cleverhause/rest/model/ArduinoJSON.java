package ru.cleverhause.rest.model;

import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/1/2017.
 */
@Getter
@Setter
public class ArduinoJSON implements Serializable {
    private List<DeviceInfo> devicesState = new ArrayList<DeviceInfo>();

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder("{\"devicesState\": [ ");
        for (DeviceInfo deviceInfo : devicesState) {
            stringBuilder.append("{");
            stringBuilder.append(deviceInfo.toString());
            stringBuilder.append("},");
        }
        stringBuilder.append("]}");
        return stringBuilder.toString();
    }

//    public static void main(String[] args) {
//        ArduinoJSON ard = new ArduinoJSON();
//        System.out.println(ard);
//    }
}
