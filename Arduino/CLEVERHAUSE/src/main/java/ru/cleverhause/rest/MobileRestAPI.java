package ru.cleverhause.rest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

/**
 * Created by Alexandr on 15.11.2017.
 */
@Controller
@RequestMapping(value = "/mobile")
public class MobileRestAPI {

    @RequestMapping(value = "/{userName}", method = RequestMethod.GET)
    public String mobileConnect(@PathVariable String userName, Model model) {
        model.addAttribute("userName", userName);
        return "redirect:/home";
    }
}
