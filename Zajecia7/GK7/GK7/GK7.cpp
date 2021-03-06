#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>
#include "colors.h"
#include "math.h"

#define GL_PI 3.14f
// rozmiary bry造 obcinania
const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near_ = 3.0;
const GLdouble far_ = 7.0;
// sta貫 do obs逝gi menu podr璚znego
enum
{
	CUTTING_PLANE, // p豉szczyzna przekroju
	POLYGON_OFFSET, // przesuni璚ie wartoi g喚bi
	EXIT // wyjie
};
// k靖 obrotu kuli
GLfloat angle = 0.0;
// k靖y obrotu szeianu
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
// wskaik rysowania p豉szczyzna przekroju
bool cutting_plane = true;
// wskaik przesuni璚ia wartoi g喚bi
bool polygon_offset = true;
// wskaik nacii璚ia lewego przycisku myszki
int button_state = GLUT_UP;
// po這瞠nie kursora myszki
int button_x, button_y;
// funkcja generuj鉍a scen� 3D
void Display()
{
	// kolor t豉 - zawarto bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// czyszczenie bufora koloru i bufora g喚bokoi
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// wyb鏎 macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();
	// przesuni璚ie uk豉du wsp馧rz璠nych szeianu do odka bry造 odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);
	// obroty szeianu
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);
	// niewielkie powi瘯szenie szeianu
	glScalef(1.15, 1.15, 1.15);
	// w章czenie testu bufora g喚bokoi
	glEnable(GL_DEPTH_TEST);
	// najpierw rysujemy kul� obracaj鉍� si� wewn靖rz szeianu;
	// z uwagi na celowy brak efekt闚 oietlenia, obr鏒 kuli
	// podkrea druga kula w wersji "szkieletowej"
	glPushMatrix();
	angle += 0.2;
	glRotatef(angle, 1.0, 1.0, 0.0);
	glColor3fv(Yellow);
	if (polygon_offset)
		glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glColor3fv(Black);
	glutWireSphere(0.5, 10, 10);
	if (polygon_offset)
		glDisable(GL_POLYGON_OFFSET_FILL);
	glPopMatrix();
	// w drugiej kolejnoi rysujemy wn皻rze szeianu;
	// rysowane s� tylko przednie strony wewn皻rznych ian
	// no i nie jest rysowana iana przednia, w kt鏎ej b璠zie otw鏎

	////////////////////////////////////

	GLfloat x, y, angle, r = 1.2f; // Przechowuj� wartoi wsp馧rz璠nych i k靖a
	int iPivot = 1; // Do oznaczania zamiennych kolor闚
	glBegin(GL_TRIANGLE_FAN);
	// Czubek stoka jest wsp鏊nym wierzcho趾iem wszystkich tr鎩k靖闚 z wachlarza
	// wysuni皻ym do g鏎y w osi z. W ten spos鏏 zamiast ko豉 powstanie sto瞠k.
	glVertex3f(0.0f, 0.0f, -2.0f);
	// Wykonujemy obr鏒 w oko這 i oznaczamy w r闚nych odst瘼ach wierzcho趾i
	// tworz鉍e wachlarz tr鎩k靖闚.
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (4.0f * GL_PI /28.0f))
	{
		// Wyliczenie wsp馧rz璠nych x i y kolejnego wierzcho趾a
		x = r*sin(angle);
		y = r*cos(angle);
		// Wybieranie koloru - zielony lub czerwony
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		// Inkrementacja zmiennej okreaj鉍ej rodzaj koloru
		iPivot++;
		// Definiowanie kolejnego wierzcho趾a w wachlarzu tr鎩k靖闚
		glVertex3f(x, y, 1.0f);
	}
	// Zakoczenie rysowania tr鎩k靖闚 sto磬a
	glEnd();

	//////////////////////////////////////////////////////


	// rysowanie p豉szczyzny otworu w szeianie
	if (cutting_plane)
	{
		// wy章czenie rysowania w buforze kolor闚
		glDrawBuffer(GL_NONE);

		// rysowanie kwadratu cz�ciowo ods豉niaj鉍ego wn皻rze szeianu
		// (kwadrat jest po這穎ny o 0,001 jednostki nad bokiem szeianu)
		glBegin(GL_QUADS);
		glVertex3f(-0.6, -0.6, 1.001);
		glVertex3f(0.6, -0.6, 1.001);
		glVertex3f(0.6, 0.6, 1.001);
		glVertex3f(-0.6, 0.6, 1.001);
		glEnd();
		// w章czenie rysowania w buforze kolor闚
		glDrawBuffer(GL_BACK);
	}
	// w豉iwy szeian z obramowaniem, kt鏎ego rysowanie wymusza brak oietlenia
	glColor3fv(Red);
	if (polygon_offset)
		glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	// Czubek stoka jest wsp鏊nym wierzcho趾iem wszystkich tr鎩k靖闚 z wachlarza
	// wysuni皻ym do g鏎y w osi z. W ten spos鏏 zamiast ko豉 powstanie sto瞠k.
	glVertex3f(0.0f, 0.0f, -2.1f);

	for (angle = 0.0f; angle < (2.0f*GL_PI ); angle += (4.0f * GL_PI / 28.0f))
	{
		// Wyliczenie wsp馧rz璠nych x i y kolejnego wierzcho趾a
		x = (0.01 + r)*sin(angle);
		y = (0.01 + r)*cos(angle);
		// Wybieranie koloru - zielony lub czerwony

		// Definiowanie kolejnego wierzcho趾a w wachlarzu tr鎩k靖闚
		glVertex3f(x, y, 1.0f);
	}
	// Zakoczenie rysowania tr鎩k靖闚 sto磬a
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	// odek wachlarza znajduje si� na pocz靖ku uk豉du wsp馧rz璠nych
	glVertex3f(0.0f, 0.0f, 1.0f);
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (4.0f * GL_PI / 28.0f))
	{
		// Wyliczenie wsp馧rz璠nych x i y kolejnego wierzcho趾a
		x = (0.01 + r)*sin(angle);
		y = (0.01 + r)*cos(angle);
		// Wybieranie koloru - zielony lub czerwony

		// Definiowanie kolejnego wierzcho趾a w wachlarzu tr鎩k靖闚
		glVertex3f(x, y, 1.0f);
	}
	// Zako鎍zenie rysowania tr鎩k靖闚 podstawy stoka
	glEnd();
	glPopMatrix();

	glColor3fv(Black);

	// skierowanie polece� do wykonania
	glFlush();
	// zamiana bufor闚 koloru
	glutSwapBuffers();
}
// zmiana wielkoi okna
void Reshape(int width, int height)
{
	// obszar renderingu - ca貫 okno
	glViewport(0, 0, width, height);
	// wyb鏎 macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();
	// parametry bry造 obcinania
	glFrustum(left, right, bottom, top, near_, far_);
	// generowanie sceny 3D
	Display();
}
// obs逝ga klawiszy funkcyjnych i klawiszy kursora
void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;
		// kursor w g鏎�
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;
		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;
		// kursor w d馧
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// obs逝ga przycisk闚 myszki
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapami皻anie stanu lewego przycisku myszki
		button_state = state;
		// zapami皻anie po這瞠nia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}
// obs逝ga ruchu kursora myszki
void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) *(x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) *(button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}
// obs逝ga menu podr璚znego
void Menu(int value)
{
	switch (value)
	{
		// p豉szczyzna przekroju
	case CUTTING_PLANE:
		cutting_plane = !cutting_plane;
		Display();
		break;
		// przesuni璚ie wartoi g喚bi
	case POLYGON_OFFSET:
		polygon_offset = !polygon_offset;
		Display();
		break;
		// wyjie
	case EXIT:
		exit(0);
	}
}
int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// rozmiary g堯wnego okna programu
	glutInitWindowSize(500, 500);
	// utworzenie g堯wnego okna programu
	glutCreateWindow("Z-bufor");
	// do章czenie funkcji generuj鉍ej scen� 3D
	glutDisplayFunc(Display);
	// do章czenie funkcji wywo造wanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);
	// do章czenie funkcji obs逝gi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);
	// obs逝ga przycisk闚 myszki
	glutMouseFunc(MouseButton);
	// obs逝ga ruchu kursora myszki
	glutMotionFunc(MouseMotion);
	// utworzenie menu podr璚znego
	glutCreateMenu(Menu);
	// menu g堯wne
	glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("P豉szczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuni璚ie wartoi g喚bi: w章cz/wy章cz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyjie", EXIT);
#else
	glutAddMenuEntry("Plaszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuniecie wartosci glebi: wlacz/wylacz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif
	// okreenie przycisku myszki obs逝guj鉍ej menu podr璚zne
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// do章czenie funkcji wywo造wanej podczas "bezczynnoi" systemu
	glutIdleFunc(Display);
	// wprowadzenie programu do obs逝gi p皻li komunikat闚
	glutMainLoop();
	return 0;
}