package com.epam.ivanov1.repository;

import com.epam.ivanov1.type.Contact;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import static org.hibernate.validator.internal.util.Contracts.assertNotNull;


/**
 *
 * Created by Aleksandr_Ivanov1 on 6/13/2017.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes=JdbcTemplate.class)
@Repository
public class ContactRepository {
    private JdbcTemplate jdbc;

    @Autowired
    public ContactRepository(JdbcTemplate jdbc) {
        this.jdbc = jdbc;
    }

    @Test
    public void cdShouldNotBeNull() {
        assertNotNull(jdbc);
    }

    public List<Contact> findAll() {
        return jdbc.query(
                "select id, firstName, lastName, phoneNumber, emailAddress " +
                        "from contacts order by lastName",
                new RowMapper<Contact>() {
                    public Contact mapRow(ResultSet rs, int rowNum)
                            throws SQLException {
                        Contact contact = new Contact();
                        contact.setId(rs.getLong(1));
                        contact.setFirstName(rs.getString(2));
                        contact.setLastName(rs.getString(3));
                        contact.setPhoneNumber(rs.getString(4));
                        contact.setEmailAddress(rs.getString(5));
                        return contact;
                    }
                });
    }
    public void save(Contact contact) {
        long count = 0;
        List<Contact> all = findAll();
        if (all != null) {
            for (Contact c : all) {
                if (c.getId() > count) {
                    count = c.getId();
                }
            }
        }
        jdbc.update(
                "insert into contacts " +
                        "(id, firstName, lastName, phoneNumber, emailAddress) " +
                        "values (?, ?, ?, ?, ?)", ++count,
                contact.getFirstName(), contact.getLastName(),
                contact.getPhoneNumber(), contact.getEmailAddress());
    }
}