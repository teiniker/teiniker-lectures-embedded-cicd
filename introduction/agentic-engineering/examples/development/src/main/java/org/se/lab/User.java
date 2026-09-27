package org.se.lab;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

import java.io.Serializable;
import java.util.Objects;

@Entity
@Table(name = "users")
public class User implements Serializable
{
    private static final long serialVersionUID = 1L;

    /*
     * Constructors
     */
    User(long id, String username, String email)
    {
        setId(id);
        setUsername(username);
        setEmail(email);
    }

    User()
    {
    }

    /*
     * Property: id:long
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;
    public long getId()
    {
        return id;
    }
    public void setId(long id)
    {
        this.id = id;
    }

    /*
     * Property: username:String
     */
    @Column(unique = true, nullable = false)
    private String username;
    public String getUsername()
    {
        return username;
    }
    public void setUsername(String username)
    {
        this.username = username;
    }

    /*
     * Property: email:String
     */
    @Column(nullable = false)
    private String email;
    public String getEmail()
    {
        return email;
    }
    public void setEmail(String email)
    {
        this.email = email;
    }

    /*
     * Object methods
     */

    @Override
    public String toString()
    {
        return getId() + "," + getUsername() + "," + getEmail();
    }

    @Override
    public boolean equals(Object o)
    {
        if (o == null || getClass() != o.getClass())
        {
            return false;
        }

        User user = (User) o;
        return id == user.id;
    }

    @Override
    public int hashCode()
    {
        return Objects.hashCode(id);
    }
}
