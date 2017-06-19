package com.ivanov1.rmi.excercise1;

import java.io.*;
import java.util.Properties;

/**
 * Created by Aleksandr_Ivanov1 on 5/15/2017.
 */
public class PropertyReader {

    private PropertyReader() {
    }

    public static Properties getInfo(String configFileName) {
        Properties properties = new Properties();
        ClassLoader classLoader = PropertyReader.class.getClassLoader();
        File file = new File(classLoader.getResource(configFileName).getFile());
        try (InputStream input = new FileInputStream(file);
             OutputStream output = new FileOutputStream(file)) {
            properties.load(input);
        } catch (FileNotFoundException fnfEx) {
            System.out.println("File not found at class com.ivanov1.rmi.excercise1.BillingServiceImpl: " + fnfEx.getMessage());
        } catch (IOException ex) {
            System.out.println("IO fail at class com.ivanov1.rmi.excercise1.BillingServiceImpl: " + ex.getMessage());
        }
        return properties;
    }
}
