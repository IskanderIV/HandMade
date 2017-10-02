package com.epam.ivanov1.type;


import lombok.Getter;
import lombok.Setter;

/**
 *
 * Created by Aleksandr_Ivanov1 on 6/13/2017.
 */
@Getter
@Setter
public class Contact {
    private Long id;
    private String firstName;
    private String lastName;
    private String phoneNumber;
    private String emailAddress;

}
