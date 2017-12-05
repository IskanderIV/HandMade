package ru.cleverhause.repository;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Repository;

import java.util.HashMap;

/**
 * Created by
 *
 * @author Aleksandr_Ivanov1
 * @date 12/2/2017.
 */
@Repository
@Scope("session")
public class UserRepository extends HashMap<String, String> {
}
