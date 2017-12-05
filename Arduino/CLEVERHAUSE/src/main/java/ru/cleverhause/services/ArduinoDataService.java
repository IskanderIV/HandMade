package ru.cleverhause.services;

import ru.cleverhause.rest.model.ArduinoJSON;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/2/2017.
 */

public interface ArduinoDataService {

    boolean put(String key, ArduinoJSON arduinoJSON);

    ArduinoJSON getLast();

    boolean delete(String key);

    ArduinoJSON find(String arduinoJsonKey);
}
