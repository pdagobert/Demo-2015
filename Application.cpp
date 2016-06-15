#include "Application.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Universe.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL2/SDL_mixer.h>
#include <algorithm>
#include <SDL2/SDL.h>

//const int windowWidth = 1980;
//const int windowHeight = 1080;
//const int windowWidth = 800;
//const int windowHeight = 600;
int windowWidth = 1440;
int windowHeight = 900;

enum PlanetType
{
	Moon,
	Earth,
	Jupiter,
	Mars,
	Saturne,
	Max
};

Application::Application() :
	Window( windowWidth, windowHeight, "Project" ),
	m_introShader( "Shaders/Mesh.vs", "Shaders/Intro.fs" ),
	m_skyboxShader( "Shaders/Skybox.vs", "Shaders/Skybox.fs" ),
	m_rocketShader( "Shaders/Mesh.vs", "Shaders/Mesh.fs" ),
	m_flares( "Shaders/Flares.vs", "Shaders/Flares.fs" ),
	m_creditShader( "Shaders/Mesh.vs", "Shaders/Credits.fs" ),
	m_quad( createUnitQuad() ),
	m_cube( createUnitCube() ),
	m_skybox(),
	m_angle( 0.0f ),
	m_moves( false ),
	m_rocket( "Rocket.obj" ),
	m_rocketTex( load2DTexture( "Textures/Rocket.png" ) ),
	m_flareTex( load2DTexture( "Textures/lens_flares.png" ) ),
	m_asteroidTex( load2DTexture( "Textures/asteroid.png" ) ),
	m_time( (float)glfwGetTime() )
{
	//windowWidth = cwindowWidth;
	//windowHeight = cwindowHeight;
	
	srand( 3040 );
	//SDL_GL_SetSwapInterval( 1 );
	glfwSwapInterval( 1 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_MULTISAMPLE );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	m_skybox.preRenderSkybox( windowWidth, windowHeight );

	m_planets.resize( PlanetType::Max );

	UVPlanet p = { 1.5f, glm::vec3( 215.0f, 10.0f, 5.0f ), 0.2f, 0.0f, load2DTexture( "Textures/moon.jpeg" ), 0.0f, glm::vec3( 0.0f, 0.0f, 0.0f ), false, 0.0f, 0.0f, 0 };
       
 	UVPlanet p2 = { 6.0f, glm::vec3( 200.0f, 0.0f, 70.0f ), 0.2f, 1.0f, load2DTexture( "Textures/earth.jpeg" ), 0.5f, glm::vec3( 0.0f, 0.4f, 1.0f ), false, 0.0f, 0.0f, 0 };
 	UVPlanet p3 = { 25.0f, glm::vec3( -525.0f, -20.0f, 0.0f ), 0.3f, 2.0f, load2DTexture( "Textures/jupiter.jpeg" ), 1.5f, glm::vec3( 0.5f, 0.25f, 0.0f ), false, 0.0f, 0.0f, 0 };
 	UVPlanet p4 = { 4.0f, glm::vec3( -350.0f, 5.0f, -125.0f ), 0.2f, 0.0f, load2DTexture( "Textures/mars.jpeg" ), 0.0f, glm::vec3( 0.0f, 0.0f, 0.0f ), false, 0.0f, 0.0f, 0 };
 	UVPlanet p5 = { 20.0f, glm::vec3( -800.0f, -185.0f, 242.0f ), 0.2f, 0.0f, load2DTexture( "Textures/saturne.jpeg" ), 0.0f, glm::vec3( 1.0f, 1.0f, 1.0f ), true, 25.0f, 50.0f, load1DTexture( "Textures/ring.png" ) };

 	m_planets[ PlanetType::Moon ] = p;
 	m_planets[ PlanetType::Earth ] = p2;
 	m_planets[ PlanetType::Jupiter ] = p3;
 	m_planets[ PlanetType::Mars ] = p4;
 	m_planets[ PlanetType::Saturne ] = p5;

	
 	m_rocketPos = m_planets[ PlanetType::Earth ].position;
 	m_asteroidPos = m_planets[ PlanetType::Jupiter ].position;
        
 	m_credits[ 0 ] = load2DTexture( "Textures/credit1.png" );
 	m_credits[ 1 ] = load2DTexture( "Textures/credit2.png" );

 	initSDLMixer();
	//glfwSetTime( 0.0 );

	m_volume = Mix_VolumeMusic( -1 );
}

void Application::initSDLMixer()
{
	int flags = MIX_INIT_MP3;
	Mix_Init( flags );

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) != 0 )
		throw std::runtime_error( "SDL Mixer init failed" );

	Mix_Music* music = Mix_LoadMUS( "Sam-Free Music _ Naked Soul.mp3" );
	if( !music )
		throw std::runtime_error( "Music not found" );

	Mix_PlayMusic( music, 0 );
}

void Application::update( float deltaTime )
{
	if( m_moves )
		m_angle += deltaTime * glm::pi< float >() * 2.0f / 10.0f;

	for( UVPlanet& p : m_planets )
		p.angle += p.rotationSpeed * deltaTime;
}

void Application::renderIntro( float time )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m_introShader.bind();
	m_introShader.setUniform( "wvp", glm::mat4() );
	m_introShader.setUniform( "time", time );

	m_quad.render( GL_TRIANGLES );
}

glm::vec3 bezierCurve( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float f )
{
	float t = 1.0f - f;
	float b0 = t * t * t;
	float b1 = 3.0f * f * t * t;
	float b2 = 3.0f * f * f * t;
	float b3 = f * f * f;

	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
}

void Application::render()
{
	float time = (float)glfwGetTime() - m_time;

	float offset = 0.0;

	if( time < 28.5f + offset )
	{
		renderIntro( time );
		return;
	}
	else
	{
		float t = time - ( 28.5f + offset );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::vec3 cameraPos;
		glm::vec3 cameraTarget;

		getCameraSetup( t, cameraPos, cameraTarget );

		if( t >= 95.0f + offset )
		{
			float t2 = ( t - ( 95.0f + offset ) ) / ( 14.0f + offset );

			int current = t2 + 0.5f;

			m_creditShader.bind();
			m_creditShader.setUniform( "wvp", glm::mat4() );
			m_creditShader.setUniform( "tex", 0 );

			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_credits[ current ] );

			m_quad.render( GL_TRIANGLES );
			return;
		}

		glm::mat4 proj = glm::perspective( glm::radians( 45.0f ), (float)windowWidth / windowHeight, 0.1f, 1000.0f );
		glm::mat4 view = glm::lookAt( cameraPos, cameraTarget, glm::normalize( glm::vec3( 0.0f, 1.0f, 0.0f ) ) );

		renderSkybox( m_skybox.getCubemap(), proj, view );

		StarGlow s = { 120.0f, glm::vec3( 0.0f ), glm::vec3( 1.0f, 1.0f, 0.0f ) };
		UniverseRenderer::getInstance().renderStarGlow( s, proj, view );

		if( t < 7.0f + offset )
			return;

		std::vector< UVPlanet > v = m_planets;

		std::sort( v.begin(), v.end(), 
			[ &cameraPos ]( UVPlanet a, UVPlanet b )
			{
				if( glm::distance( a.position, cameraPos ) > glm::distance( b.position, cameraPos ) )
					return true;

				return false;
			} );
		

		for( const UVPlanet& p : v )
			UniverseRenderer::getInstance().renderUVPlanet( p, proj, view, cameraPos );

		renderRocket( t, proj, view );

		renderLensFlares( s.position, proj, view );

		renderAsteroid( proj, view );
	}
}

float offsets[ 6 ] = { 1.0f, 1.25f, 1.1f, 1.5f, 1.6f, 1.7f };
float sizes[ 6 ] = { 1.3f, 1.0f, 1.75f, 0.65f, 0.9f, 0.45f };
float type[ 6 ] = { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

void Application::renderLensFlares( const glm::vec3& starPos, const glm::mat4& proj, const glm::mat4& view )
{
	glm::vec4 screenSpacePos = proj * view * glm::vec4( starPos, 1.0f );
	screenSpacePos /= screenSpacePos.w;

	if( screenSpacePos.x > 1.0f || screenSpacePos.x < -1.0f ||
		screenSpacePos.y > 1.0f || screenSpacePos.y < -1.0f ||
		screenSpacePos.z > 1.0f || screenSpacePos.z < -1.0f )
	{
		return;
	}

	glBlendFunc( GL_ONE, GL_ONE );

	m_flares.bind();
	m_flares.setUniform( "center", starPos );
	m_flares.setUniform( "wvp", proj * view );
	m_flares.setUniform( "tex", 0 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_flareTex );

	for( unsigned int i = 0; i < 6; i++ )
	{
		m_flares.setUniform( "scale", sizes[ i ] );
		m_flares.setUniform( "flareOffset", offsets[ i ] );
		m_flares.setUniform( "textureType", type[ i ] );

		m_quad.render( GL_TRIANGLES );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Application::renderSkybox( GLuint cubemap, const glm::mat4& proj, const glm::mat4& view )
{
	m_skyboxShader.bind();
	m_skyboxShader.setUniform( "wvp", proj * glm::mat4( glm::mat3( view ) ) );
	m_skyboxShader.setUniform( "skybox", 0 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap );

	m_cube.render( GL_TRIANGLES );
}

float smootherStep( float x )
{
	return x * x * x * ( x * ( x * 6.0f - 15.0f ) + 10.0f );
}

void Application::renderRocket( float time, const glm::mat4& proj, const glm::mat4& view )
{
	if( time < 20.0f || time >= 28.0f )
		return;

	glm::vec3 p0, p1, p2, p3;
	p0 = m_planets[ PlanetType::Earth ].position;
	p3 = m_planets[ PlanetType::Moon ].position;
	p1 = p0;
	p2 = glm::vec3( p0.x + ( p3.x - p0.x ) * 0.19f, p0.y, p0.z + ( p0.z - p3.z ) * 0.22f );

	float t = ( time - 20.0f ) / 8.0f;
	static glm::vec3 lastPos = m_rocketPos;

	m_rocketPos = bezierCurve( p0, p1, p2, p3, t * 0.95f );

	glm::vec3 normal = glm::normalize( m_rocketPos - lastPos );

	float angle = glm::dot( glm::vec3( 1.0f, 0.0f, 0.0f ), normal );
	glm::vec3 vec = glm::normalize( glm::cross( glm::vec3( 0.0f, 1.0f, 0.0f ), normal ) );

	glm::quat orientation = glm::angleAxis( glm::acos( angle ), vec );

	glm::mat4 world;
	world = glm::translate( world, m_rocketPos );
	world = world * glm::mat4_cast( orientation );
	world = glm::scale( world, glm::vec3( 0.2f ) );

	m_rocketShader.bind();
	m_rocketShader.setUniform( "wvp", proj * view * world );
	m_rocketShader.setUniform( "world", world );
	m_rocketShader.setUniform( "tex", 0 );
	m_rocketShader.setUniform( "lightDir", glm::normalize( -glm::vec3( 200.0f, 0.0f, 70.0f ) ) );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_rocketTex );

	m_rocket.render( GL_TRIANGLES );

	lastPos = m_rocketPos;
}

void Application::renderAsteroid( const glm::mat4& proj, const glm::mat4& view )
{
	Asteroid a = { 0.5f, m_asteroidPos, ( (float)glfwGetTime() - m_time ) / 10.0f, m_asteroidTex };
	UniverseRenderer::getInstance().renderAsteroid( a, proj, view );
}

void Application::getCameraSetup( float time, glm::vec3& cameraPos, glm::vec3& cameraTarget )
{
	float pos = 5.0 + time * 25.0f;

	if( time < 7.0f )
	{
		cameraPos = glm::vec3( pos * glm::cos( time ), 0.0f, pos * glm::sin( time ) );
		cameraTarget = glm::vec3( 0.0f );
	}
	else if( time < 20.0f )
	{
		float t = ( time - 7.0f ) / 13.0f;

		glm::vec3 p1 = m_planets[ PlanetType::Moon ].position;
 		glm::vec3 p2 = m_planets[ PlanetType::Earth ].position;

		cameraTarget = glm::mix( p1, p2, t );
		cameraPos = glm::mix( glm::vec3( 205.0f, 3.0f, 15.0f ), glm::vec3( 205.0f, 3.0f, 35.0f ), t );
	}
	else if( time < 28.0f )
	{
		cameraTarget = m_rocketPos;
		cameraPos = glm::vec3( 205.0f, 3.0f, 35.0f );
	}
	else if( time < 44.0f )
	{
		float t = ( time - 28.0f ) / 16.0f;

		glm::vec3 basePos = m_planets[ PlanetType::Jupiter ].position - glm::vec3( -40.0f, 40.0f, 80.0f );

		cameraPos = basePos;
		cameraTarget = glm::mix( m_planets[ PlanetType::Jupiter ].position, glm::vec3( 0.0f, -60.0f, 0.0f ), glm::smoothstep( 0.0f, 1.0f, t ) );
	}
	else if( time < 56.0f )
	{
		float t = ( time - 44.0f ) / 12.0f;

		glm::vec3 basePos = m_planets[ PlanetType::Jupiter ].position - glm::vec3( -40.0f, 40.0f, 80.0f );
		glm::vec3 destPos = glm::vec3( -330.0f, 0.0f, -95.0f );

		cameraTarget = glm::mix( glm::vec3( 0.0f, -60.0f, 0.0f ), m_planets[ PlanetType::Mars ].position, smootherStep( t ) );
		cameraPos = glm::mix( basePos, destPos, smootherStep( t ) );
	}
	else if( time < 68.0f )
	{
		float t = ( time - 56.0f ) / 12.0f;

		glm::vec3 basePos = glm::vec3( -330.0f, 0.0f, -95.0f );
		glm::vec3 destPos = m_planets[ PlanetType::Mars ].position + glm::vec3( -500.0f, 95.0f, -170.0f );

		cameraPos = glm::mix( basePos, destPos, t );
		cameraTarget = glm::mix( m_planets[ PlanetType::Mars ].position, m_planets[ PlanetType::Jupiter ].position, smootherStep( t ) );
	}
	else if( time < 81.8f )
	{
		float t = ( time - 69.0f ) / ( 81.8f - 68.0f );

		glm::vec3 asteroidPos = glm::mix( m_planets[ PlanetType::Jupiter ].position, m_planets[ PlanetType::Saturne ].position, 0.3f + t / 2.0f );
		m_asteroidPos = asteroidPos;

		glm::vec3 normal = glm::normalize( m_asteroidPos );
		cameraPos = m_asteroidPos - glm::normalize( normal ) * 6.0f;
		cameraTarget = m_asteroidPos;
	}
	else if( time < 95.0f )
	{
		float t = ( time - 80.0f ) / ( 95.0f - 81.8f );

		cameraTarget = glm::mix( m_asteroidPos, m_planets[ PlanetType::Saturne ].position, t );
		cameraPos = glm::mix( glm::vec3( -820.0f, -165.0f, 302.0f ), glm::vec3( -620.0f, -165.0f, 255.0f ), t );
	}
	else if( time < 109.0f )
	{
		float t = ( time - 95.0f ) / 14.0f;
		Mix_VolumeMusic( m_volume - t * m_volume );
	}
	else
	{
		//close();
		exit( 0 );
	}
}

/*void Application::keyPressed( int key )
{
	if( key == GLFW_KEY_SPACE )
	{
		static bool wireframe = false;
		wireframe = !wireframe;

		if( wireframe )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	else if( key == GLFW_KEY_E )
		m_moves = !m_moves;
}*/
