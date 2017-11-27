package ru.cleverhause.rest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by Alexandr on 15.11.2017.
 */
@Controller
@RequestMapping(value = "/arduino")
public class ArduinoRestAPI {

    @RequestMapping(value = "/{userName}", method = RequestMethod.GET)
    public String arduinoConnect(@PathVariable String userName, ModelMap model) {
        model.addAttribute("userName", userName);
        return "redirect:/index";
    }
}
