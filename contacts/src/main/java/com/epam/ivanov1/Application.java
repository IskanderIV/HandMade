package com.epam.ivanov1;

import com.epam.ivanov1.config.JpaConfig;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Import;

/**
 *
 * Created by
 * @author Aleksandr_Ivanov1 on 6/13/2017.
 */
@ComponentScan
@EnableAutoConfiguration
@Import({ JpaConfig.class })
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
