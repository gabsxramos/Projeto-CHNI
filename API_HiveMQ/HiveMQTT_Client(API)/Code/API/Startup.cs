//using Microsoft.AspNetCore.Builder;
//using Microsoft.AspNetCore.Hosting;
//using Microsoft.Extensions.Configuration;
//using Microsoft.Extensions.DependencyInjection;
//using Microsoft.Extensions.Hosting;

namespace Startup // Certifique-se de que o namespace corresponde ao do seu projeto
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // Este método é chamado pelo runtime para adicionar serviços ao contêiner de injeção de dependência
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers();
            
            // Registra o MqttService como um serviço em segundo plano (hosted service)
            services.AddHostedService<MqttService>();
        }

        // Este método é chamado pelo runtime para configurar o pipeline de requisição HTTP
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseRouting();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });
        }
    }
}
