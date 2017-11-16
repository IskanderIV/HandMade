package ru.cleverhause.rest;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

/**
 * Created by Alexandr on 15.11.2017.
 */
//@RestController
@RequestMapping(value="/")
public class ArduinoRestAPI {

    @RequestMapping(method = RequestMethod.GET)
    public String getDataFromArduinoClient() {
        return "index";
    }
}
