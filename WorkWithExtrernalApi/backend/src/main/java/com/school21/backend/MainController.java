package com.school21.backend;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MainController {
    @GetMapping("/news/linux")
    public String linuxNews(){
        NewsService newsService = new NewsService();
        return newsService.getLinux();
    }

    @GetMapping("/news/linux/ru")
    public String linuxNewsRu(){
        NewsService newsService = new NewsService();
        return newsService.getLinuxRu();
    }

    @GetMapping("/news/development/ru")
    public String developmentNewsRu(){
        NewsService newsService = new NewsService();
        return newsService.getDevelopment();
    }

    @GetMapping("/news/science/titles")
    public String scienceRuTitles(){
        NewsService newsService = new NewsService();
        return newsService.getScienceRuTitles();
    }
}
