# web服务器数据链路

![image-20230325142459955](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230325142459955.png)

| 服务器   | 通讯协议                                | 应用场景                                    | 上游数据处理中心                                             | 下游数据处理中心                 |
| -------- | --------------------------------------- | ------------------------------------------- | ------------------------------------------------------------ | -------------------------------- |
| Apache   | HTTP、HTTPS                             | **Web服务器**，支持CGI、SSI等技术           | 客户端                                                       | 应用服务器                       |
| Nginx    | HTTP、HTTPS                             | **Web服务器**，支持反向代理和负载均衡等技术 | 客户端                                                       | 应用服务器                       |
| IIS      | HTTP、HTTPS                             | **Web服务器**，支持ASP.NET等技术            | 客户端                                                       | 应用服务器                       |
| Tomcat   | HTTP、HTTPS、AJP/1.3                    | **Java应用服务器**，支持Servlet和JSP等技术  | Web服务器（如Apache）或客户端（如浏览器）                    | 数据库服务器                     |
| JBoss    | HTTP、HTTPS、AJP/1.3、RMI/IIOP          | **Java应用服务器**，支持EJB等技术           | Web服务器（如Apache）或客户端（如浏览器）                    | 数据库服务器                     |
| Weblogic | HTTP、HTTPS、T3/T3S（Weblogic专有协议） | **Java应用服务器**，支持EJB等技术           | Web服务器（如Apache）或客户端（如浏览器）                    | 数据库服务器                     |
| MySQL    | MySQL专有协议，TCP/IP协议等。           | **关系型数据库**管理系统。                  | 应用程序或Web应用程序（如PHP程序）或客户端（如Navicat）或Web服务器（如Apache） | 数据库客户端或其他数据库管理系统 |
| Oracle   | Oracle专有协议，TCP/IP协议等。          | **关系型数据库**管理系统。                  | 应用程序或Web应用程序（如Java程序）或客户端（如SQL Developer）或Web服务器（如Apache） | 数据库客户端或其他数据库管理系统 |

