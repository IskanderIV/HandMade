package ru.cleverhause.services.impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import ru.cleverhause.repository.ArduinoDataRepository;
import ru.cleverhause.rest.model.ArduinoJSON;
import ru.cleverhause.services.ArduinoDataService;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/2/2017.
 */
@Service
public class ArduinoDataServiceImpl implements ArduinoDataService{
    @Autowired
    private ArduinoDataRepository arduinoDataRepository;

    @Override
    public boolean put(String key, ArduinoJSON arduinoJSON) {
        arduinoDataRepository.put(key, arduinoJSON);
        System.out.println(arduinoDataRepository);
        arduinoDataRepository.setLast(arduinoJSON);
        System.out.println("PUT operation");
        return true;
    }

    @Override
    public ArduinoJSON getLast() {
        System.out.println("GETLAST operation");
        return arduinoDataRepository.getLast();
    }

    @Override
    public boolean delete(String arduinoJsonKey) {
        arduinoDataRepository.remove(arduinoJsonKey);
        System.out.println("DELETE operation");
        return find(arduinoJsonKey) == null;
    }

    public ArduinoJSON find(String arduinoJsonKey) {
        System.out.println("FIND operation");
        if (arduinoDataRepository.isEmpty()) {
            return null;
        }
        return arduinoDataRepository.get(arduinoJsonKey);
    }
}
