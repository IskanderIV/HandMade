package ru.cleverhause.repository;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Repository;
import ru.cleverhause.rest.model.ArduinoJSON;

import java.util.HashMap;
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
public class ArduinoDataRepository extends TreeMap<String, ArduinoJSON> {
    private ArduinoJSON last;

    public ArduinoDataRepository () {
        super();
    }
}
