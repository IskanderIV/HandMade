package ru.cleverhause.rest.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.io.Serializable;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/2/2017.
 */
@NoArgsConstructor
@AllArgsConstructor
@Getter
@Setter
public class DeviceInfo implements Serializable {
    private int deviceId;
    private String deviceData;

    @Override
    public String toString() {
        return "DeviceInfo{" +
                "deviceId=" + deviceId +
                ", deviceData='" + deviceData + '\'' +
                '}';
    }
}
