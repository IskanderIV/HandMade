package ru.cleverhause.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 11/27/2017.
 */

@Controller
public class HomePage {

    @RequestMapping(value = {"/"}, method = RequestMethod.GET)
    public String index() {
        return "home";
    }

    @RequestMapping(value = {"/{page}"}, method = RequestMethod.GET)
    public String anyOtherPage(@PathVariable String page) {
        return page;
    }
}