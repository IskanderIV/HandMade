package ru.cleverhause.rest.model;

import lombok.Getter;
import lombok.Setter;
import org.springframework.stereotype.Repository;

import java.util.TreeMap;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/2/2017.
 */
@Repository
@Getter
@Setter
public class User extends TreeMap<String, ArduinoJSON> {
}
