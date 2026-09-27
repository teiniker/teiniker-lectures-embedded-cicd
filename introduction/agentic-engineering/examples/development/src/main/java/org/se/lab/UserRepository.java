package org.se.lab;

import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<User, Long>
{
    // Additional query methods (if needed) can be defined here.
}
