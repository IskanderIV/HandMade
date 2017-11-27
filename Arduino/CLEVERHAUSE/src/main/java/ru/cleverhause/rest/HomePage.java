package ru.cleverhause.rest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 11/27/2017.
 */

@Controller
public class HomePage {
//ModelAndView повлиял на отображение
    @RequestMapping(value = {"/","/index"}, method = RequestMethod.GET)
    public String arduinoConnect(Model model) {
        model.addAttribute("h", "23");
        return "index";
    }
}