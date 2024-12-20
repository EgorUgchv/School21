package com.school21.backend;

import org.json.JSONArray;
import org.json.JSONException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import org.json.JSONObject;

@Service
public class NewsService {
    @Autowired
    private RestTemplate restTemplate;
    private static final String newsApi = "https://newsapi.org/v2/everything?q=Linux&apiKey=00b9af8458994d8a85cf9cdc380b91ac";
    public String getLinux(){
        var url = "https://newsapi.org/v2/everything?" +
                "q=Linux&" +
                "sortBy=popularity&" +
                "apiKey=00b9af8458994d8a85cf9cdc380b91ac";

        RestTemplate restTemplate = new RestTemplate();
        String json = restTemplate.getForObject(url, String.class);
        return json;
    }

    public String getLinuxRu(){
        var url = "https://newsapi.org/v2/everything?" +
                "q=Linux&" +
                "language=ru&" +
                "pageSize=10" +
                "page=3&" +
                "sortBy=popularity&" +
                "apiKey=00b9af8458994d8a85cf9cdc380b91ac";

        RestTemplate restTemplate = new RestTemplate();
        String json = restTemplate.getForObject(url, String.class);
        return json;
    }
    public String getDevelopment(){
        var url = "https://newsapi.org/v2/everything?" +
                "q=Разработка&" +
                "language=ru&" +
                "from=2024-12-05&" +
                "apiKey=00b9af8458994d8a85cf9cdc380b91ac";

        RestTemplate restTemplate = new RestTemplate();
        String json = restTemplate.getForObject(url, String.class);
        return json;
    }
public String getScienceRuTitles() throws JSONException {
    var url = "https://newsapi.org/v2/everything?" +
            "q=наука&" +
            "apiKey=00b9af8458994d8a85cf9cdc380b91ac";

    RestTemplate restTemplate = new RestTemplate();
    String json = restTemplate.getForObject(url, String.class);
    JSONObject jsonObject = new JSONObject(json);
    JSONArray articles = jsonObject.getJSONArray("articles");
    StringBuilder titles = new StringBuilder();
    for (int i = 0; i < articles.length(); i++) {
        JSONObject article = articles.getJSONObject(i);
        titles.append(article.getString("title")).append("\n");
    }

    return titles.toString();
}
    }

