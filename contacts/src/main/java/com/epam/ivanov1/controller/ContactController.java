package com.epam.ivanov1.controller;

import com.epam.ivanov1.type.Contact;
import com.epam.ivanov1.repository.ContactRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import java.util.List;
import java.util.Map;

/**
 *
 * Created by
 * @author Aleksandr_Ivanov1
 * on 6/13/2017.
 */
@Controller
@PropertySource("classpath:/1.properties")
@RequestMapping("/")
public class ContactController {
    private ContactRepository contactRepo;

    @Autowired
    private Environment env;

    @Autowired
    public ContactController (ContactRepository contactRepo) {
        this.contactRepo = contactRepo;
    }

    @RequestMapping(method = RequestMethod.GET)
    public String home(Map<String, Object> model) {
        System.out.println("[ContactRepository]: " + env.getProperty("a"));
        List<Contact> contacts = contactRepo.findAll();
        model.put("contacts", contacts);
        return "home";
    }

    @RequestMapping(method=RequestMethod.POST)
    public String submit(Contact contact) {
        contactRepo.save(contact);
        return "redirect:/";
    }
}
