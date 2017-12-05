package ru.cleverhause.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import ru.cleverhause.repository.ArduinoDataRepository;
import ru.cleverhause.rest.model.ArduinoJSON;
import ru.cleverhause.services.ArduinoDataService;

import java.util.Calendar;
import java.util.Date;

/**
 * Created by Alexandr on 15.11.2017.
 */
@Controller
@RequestMapping(value = "/arduino")
public class ArduinoRestAPI {

    @Autowired
    private ArduinoDataRepository arduinoRepo;
    @Autowired
    private ArduinoDataService arduinoDataService;

    @RequestMapping(value = "/data", method = RequestMethod.PUT, produces = MediaType.APPLICATION_JSON_VALUE, consumes = MediaType.APPLICATION_JSON_VALUE)
    public @ResponseBody ArduinoJSON fromArduino(@RequestBody ArduinoJSON fromArduinoJson) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(new Date());
        String arduinoDataKey = calendar.getTime().toString();
        System.out.println("--------------///////////////////------------------");
        Boolean putResult = arduinoDataService.put(arduinoDataKey, fromArduinoJson);
        return putResult ? fromArduinoJson : null;
    }

    @RequestMapping(value = "/data", method = RequestMethod.GET, produces = MediaType.APPLICATION_JSON_VALUE, consumes = MediaType.APPLICATION_JSON_VALUE)
    public @ResponseBody ArduinoJSON getLastArduinoData() {
        return arduinoDataService.getLast();
    }

//    @ExceptionHandler
//    public @RequestBody
}
