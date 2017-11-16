package ru.cleverhause;

import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;
import org.springframework.web.servlet.view.InternalResourceViewResolver;

/**
 * Created by Alexandr on 15.11.2017.
 */
@EnableWebMvc
@Configuration
@PropertySource(value={"classpath:application.properties"})
public class AppConfig extends AnnotationConfigApplicationContext {

    @Bean
    public PropertyPlaceholderConfigurer placeHolderConfigurer() {
        return new PropertyPlaceholderConfigurer();
    }

    public InternalResourceViewResolver viewResolver() {
        InternalResourceViewResolver viewResolver = new InternalResourceViewResolver();
        viewResolver.setPrefix("/WEB-INF/jsp/");
        viewResolver.setSuffix(".jsp");
        return viewResolver;
    }
}
