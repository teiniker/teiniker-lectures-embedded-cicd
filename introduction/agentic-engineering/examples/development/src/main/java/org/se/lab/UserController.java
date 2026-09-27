package org.se.lab;

import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/v1/users")
public class UserController
{
    @Autowired  // Dependency Injection
    private UserRepository userRepository;

    @PostMapping
    public ResponseEntity<User> insert(
            @Valid @RequestBody UserRequest request)
    {
        User user = new User();
        user.setUsername(request.getUsername());
        user.setEmail(request.getEmail());

        try
        {
            User savedUser = userRepository.save(user);
            return ResponseEntity
                    .status(HttpStatus.CREATED)
                    .body(savedUser);
        }
        catch (DataIntegrityViolationException e)
        {
            return ResponseEntity.status(HttpStatus.CONFLICT).build();
        }
    }
}
