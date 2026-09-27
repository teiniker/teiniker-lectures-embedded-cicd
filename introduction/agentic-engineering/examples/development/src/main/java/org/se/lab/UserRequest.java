package org.se.lab;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;

/*
 * Request payload for creating or updating a user.
 * There is no id field, so a payload containing an explicit
 * "id" key is rejected as a structural validation failure.
 */
@JsonIgnoreProperties(ignoreUnknown = false)
public class UserRequest
{
    /*
     * Property: username:String
     */
    @NotNull
    @Pattern(regexp = "[a-z0-9]+")
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
    @NotNull
    @Email
    private String email;
    public String getEmail()
    {
        return email;
    }
    public void setEmail(String email)
    {
        this.email = email;
    }
}
