import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class back_tier extends HttpServlet {
 
  private String message;

  public void init() throws ServletException
  {
  }

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
            throws ServletException, IOException
  {
      // Set response content type
      response.setContentType("text/html");

      // Actual logic goes here.
      PrintWriter out = response.getWriter();
      out.println("Hello from inside Tomcat");
  }
  
  public void destroy()
  {
      // do nothing.
  }
}
