#include <iostream>
#include <time.h>
#include <stdio.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "float.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "instance.h"
#include "mesh.h"
#include "raster.h"
#include "camera.h"
#include "color.h"
#include "texture.h"

#include <cstdio>
#include <vector>
#include "spline.h"


using namespace std;

int init(int width, int height) {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Path Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	return 0;
}


void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{

	// World
	hittable_list world;
	list_light worldlight;

	material* light = new area_light();
	light->texture = new constant_texture(color(20, 20, 20));

	//lampada a muro
	sphere* sphere_model = new sphere();
	auto light_position = make_shared<instance>(sphere_model, light);
	light_position->scale(0.5f, 0.5f, 0.5f);
	light_position->translate(42.856, 12.54, -10.0);
	world.add(light_position);


	//MATERIALI
	// Vetro Marrone
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è colorato di marrone (vedi c_in)
	dielectric* m_vetro_marrone_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione
	m_vetro_marrone_in_aria->eta_in = 1.51;
	m_vetro_marrone_in_aria->eta_out = 1.0;
	// definisco i colori di filtraggio
	m_vetro_marrone_in_aria->c_out = color(1.0, 1.0, 1.0);
	m_vetro_marrone_in_aria->c_in = color( pow(139.0 / 255.0, 0.1), pow(69.0 / 255.0, 0.1), pow(19.0 / 255.0, 0.1));

	//Vetro verde
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è colorato di verde (vedi c_in)
	dielectric* m_vetro_verde_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione
	m_vetro_verde_in_aria->eta_in = 1.51;
	m_vetro_verde_in_aria->eta_out = 1.0;
	// definisco i colori di filtraggio
	m_vetro_verde_in_aria->c_out = color(1.0, 1.0, 1.0);
	m_vetro_verde_in_aria->c_in = color(pow(34.0 / 255.0, 0.2), pow(139.0 / 255.0, 0.2), pow(34.0 / 255.0, 0.2));

	//Vetro viola
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è colorato di viola (vedi c_in)
	dielectric* m_vetro_viola_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione
	m_vetro_viola_in_aria->eta_in = 1.51;
	m_vetro_viola_in_aria->eta_out = 1.0;
	// definisco i colori di filtraggio
	m_vetro_viola_in_aria->c_out = color(1.0, 1.0, 1.0);
	m_vetro_viola_in_aria->c_in = color(pow(138.0 / 255.0, 0.1), pow(43.0 / 255.0, 0.1), pow(226.0 / 255.0, 0.1));

	//Vetro celeste
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è colorato di celeste (vedi c_in)
	dielectric* m_vetro_caleste_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_caleste_in_aria->eta_in = 1.51; // vetro
	m_vetro_caleste_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetro_caleste_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetro_caleste_in_aria->c_in = color(pow(153.0 / 255.0, 0.7), pow(203.0 / 255.0, 0.7), 1.0);

	//Vetro rosa
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è colorato di rosa (vedi c_in)
	dielectric* m_vetro_rosa_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_rosa_in_aria->eta_in = 1.51; // vetro
	m_vetro_rosa_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetro_rosa_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetro_rosa_in_aria->c_in = color(pow(198.0 / 255.0, 0.3), pow(146.0 / 255.0, 0.3), pow(148.0 / 255.0, 0.3));

	//Vetro oro
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è color oro/giallo (vedi c_in)
	dielectric* m_vetro_oro_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_oro_in_aria->eta_in = 1.51; // vetro
	m_vetro_oro_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetro_oro_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetro_oro_in_aria->c_in = color(pow(0.65, 0.2), pow(0.45, 0.2), 0.0); //= color(205.0 / 255.0, 164.0 / 255.0, 52.0 / 255.0);

	//Vetro grigio
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro è color grigio (vedi c_in)
	dielectric* m_vetro_grigio_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_grigio_in_aria->eta_in = 1.51; // vetro
	m_vetro_grigio_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetro_grigio_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetro_grigio_in_aria->c_in = color(pow(120.0 / 255.0, 0.3), pow(120.0 / 255.0, 0.3), pow(120.0 / 255.0, 0.3));

	//Vetro normale
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro non ha colore (vedi c_in)
	dielectric* m_vetro_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_in_aria->eta_in = 1.51; // vetro
	m_vetro_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetro_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetro_in_aria->c_in = color(0.85, 0.85, 0.85);

	//Vetro scuro
	// materiale per le superfici che dentro sono in vetro e fuori sono circondate da aria
	// inoltre questo vetro non ha colore (vedi c_in)
	dielectric* m_vetroS_in_aria = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetroS_in_aria->eta_in = 1.51; // vetro
	m_vetroS_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_vetroS_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_vetroS_in_aria->c_in = color(0.4, 0.4, 0.4);

	//vetro normale in acqua
	// materiale per le superfici che dentro sono in vetro e fuori sono di acqua
	// inoltre questo vetro non ha colore (vedi c_in)
	dielectric* m_vetro_in_acqua = new dielectric(1.51);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_vetro_in_acqua->eta_in = 1.51; // vetro
	m_vetro_in_acqua->eta_out = 1.33; // acqua
	// definisco i colori di filtraggio
	m_vetro_in_acqua->c_out = color(pow(153.0 / 255.0, 0.009), pow(203.0 / 255.0, 0.009), 1.0);
	m_vetro_in_acqua->c_in = color(0.85, 0.85, 0.85);

	//acqua in aria
	// materiale per le superfici che dentro sono in acqua e fuori hanno l'aria
	dielectric* m_acqua_in_aria = new dielectric(1.33);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_acqua_in_aria->eta_in = 1.33; // acqua
	m_acqua_in_aria->eta_out = 1.0; // aria
	// definisco i colori di filtraggio
	m_acqua_in_aria->c_out = color(1.0, 1.0, 1.0);
	m_acqua_in_aria->c_in = color(pow(153.0 / 255.0, 0.009), pow(203.0 / 255.0, 0.009), 1.0);

	//Plastica trasparente in aria
	// materiale per le superfici che dentro sono in plastica trasparente e fuori sono circondate da aria
	// inoltre questo vetro non ha colore (vedi c_in)
	dielectric* m_plastica_in_aria = new dielectric(1.80);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_plastica_in_aria->eta_in = 1.80; // plastica
	m_plastica_in_aria->eta_out = 1.0; // vuoto
	// definisco i colori di filtraggio
	m_plastica_in_aria->c_out = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore
	m_plastica_in_aria->c_in = color(0.9, 0.9, 0.9);

	//Aria in plastica trasparente
	// materiale per le superfici che dentro sono in aria e fuori sono circondate da plastica trasparente
	// inoltre questo vetro non ha colore (vedi c_in)
	dielectric* m_aria_in_plastica = new dielectric(1.0);
	// definiamo gli indici di rifrazione all'interno ed all'esterno del materiale 
	m_aria_in_plastica->eta_in = 1.0; // vuoto
	m_aria_in_plastica->eta_out = 1.80; // plastica
	// definisco i colori di filtraggio
	m_aria_in_plastica->c_out = color(0.9, 0.9, 0.9);
	m_aria_in_plastica->c_in = color(1.0, 1.0, 1.0); // bianco perchè l'aria non modifica il colore


	//NATIVITA'

	mesh* gesu = new mesh("models/Gesu con mangiatoia.obj", "models/");

	// creo un'istanza solo al fine di usarne le matrici
	auto instance_di_supporto = make_shared<instance>(gesu, m_vetro_oro_in_aria);
	instance_di_supporto->translate(-0.5f, 0.0f, -0.7f);
	instance_di_supporto->scale(1.5, 1.6, 2.0);
	instance_di_supporto->translate(-7.0f, 5.4f, 5.0f);
	instance_di_supporto->translate(5.0f, -10.4f, -9.7f);
	instance_di_supporto->rotate_y(17.0);
	// ho replicato le trasformazioni che vengono fatte alla stalla
	// ora le applico a tutti i personaggi della natività

	auto instance_ptrGesu = make_shared<instance>(gesu, m_vetro_oro_in_aria);
	float scala = 2.0;
	instance_ptrGesu->translate(0.0f, 0.0f, -0.15f);
	instance_ptrGesu->scale(scala, scala, scala);
	instance_ptrGesu->translate(0.0f, 0.0f, scala * 0.15f);
	instance_ptrGesu->translate(0.0f, -0.5f, 0.0f);
	instance_ptrGesu->scale(6.0, 6.0, 6.0);
	instance_ptrGesu->setCMat(instance_di_supporto->getCMat()* instance_ptrGesu->getCMat());
	instance_ptrGesu->setIMat(instance_ptrGesu->getIMat()* instance_di_supporto->getIMat());
	instance_ptrGesu->InOmbrabile = true;
	instance_ptrGesu->FaOmbra = true;
	world.add(instance_ptrGesu);

	mesh* madonna = new mesh("models/virginMary.obj", "models/");
	auto instance_ptrMadonna = make_shared<instance>(madonna, m_vetro_rosa_in_aria);
	float scala2 = 1.75;
	instance_ptrMadonna->translate(0.1f, 0.0f, -0.05f);
	instance_ptrMadonna->scale(scala2, scala2, scala2);
	instance_ptrMadonna->translate(-scala2 / 10.0f, 0.0f, scala2 / 20.0f);
	instance_ptrMadonna->translate(-0.05f, -0.5f, 0.0f);
	instance_ptrMadonna->translate(0.0f, -0.0f, 0.12f);
	instance_ptrMadonna->scale(6.0, 6.0, 6.0);
	instance_ptrMadonna->setCMat(instance_di_supporto->getCMat()* instance_ptrMadonna->getCMat());
	instance_ptrMadonna->setIMat(instance_ptrMadonna->getIMat()* instance_di_supporto->getIMat());
	instance_ptrMadonna->InOmbrabile = true;
	instance_ptrMadonna->FaOmbra = true;
	world.add(instance_ptrMadonna);

	mesh* giuseppe = new mesh("models/Giuseppe.obj", "models/");
	auto instance_ptrGiuseppe = make_shared<instance>(giuseppe, m_vetro_caleste_in_aria);
	float scala3 = 1.9;
	instance_ptrGiuseppe->translate(-0.11f, 0.0f, -0.05f);
	instance_ptrGiuseppe->scale(scala3, scala3, scala3);
	instance_ptrGiuseppe->rotate_y(0.0);
	instance_ptrGiuseppe->translate(scala3 * 0.11f, 0.0f, scala3 / 20.0f);
	instance_ptrGiuseppe->translate(0.07f, -0.48f, 0.15f);
	instance_ptrGiuseppe->translate(-0.07f, -0.0f, -0.20f);
	instance_ptrGiuseppe->scale(6.0, 6.0, 6.0);
	instance_ptrGiuseppe->setCMat(instance_di_supporto->getCMat()* instance_ptrGiuseppe->getCMat());
	instance_ptrGiuseppe->setIMat(instance_ptrGiuseppe->getIMat()* instance_di_supporto->getIMat());
	instance_ptrGiuseppe->InOmbrabile = true;
	instance_ptrGiuseppe->FaOmbra = true;
	world.add(instance_ptrGiuseppe);

	mesh* bue = new mesh("models/bue.obj", "models/");
	auto instance_ptrBue = make_shared<instance>(bue, m_vetroS_in_aria);
	instance_ptrBue->translate(-0.1f, 0.0f, 0.07f);
	instance_ptrBue->scale(scala2, scala2, scala2);
	instance_ptrBue->rotate_y(-15.0);
	instance_ptrBue->translate(scala2 * 0.1f, -0.0f, -scala2 * 0.07f);
	instance_ptrBue->translate(0.15f, -0.48f, -0.020f);
	instance_ptrBue->translate(0.0f, -0.0f, 0.4f);
	instance_ptrBue->scale(6.0, 6.0, 6.0);
	instance_ptrBue->setCMat(instance_di_supporto->getCMat()* instance_ptrBue->getCMat());
	instance_ptrBue->setIMat(instance_ptrBue->getIMat()* instance_di_supporto->getIMat());
	instance_ptrBue->InOmbrabile = true;
	instance_ptrBue->FaOmbra = true;
	world.add(instance_ptrBue);

	mesh* asinello = new mesh("models/asinello.obj", "models/");
	auto instance_ptrAsinello = make_shared<instance>(asinello, m_vetro_grigio_in_aria);
	// ingrandimento
	instance_ptrAsinello->translate(0.11f, 0.0f, 0.07f);
	instance_ptrAsinello->scale(scala3, scala3, scala3);
	instance_ptrAsinello->rotate_y(-25.0);
	instance_ptrAsinello->translate(-scala3 * 0.11f, 0.0f, -scala3 * 0.07f);
	instance_ptrAsinello->translate(-0.1f, -0.5f, -0.05f);
	instance_ptrAsinello->translate(0.15f, -0.0f, -0.07f);
	instance_ptrAsinello->rotate_y(10);
	instance_ptrAsinello->scale(6, 6, 6);
	instance_ptrAsinello->setCMat(instance_di_supporto->getCMat()* instance_ptrAsinello->getCMat());
	instance_ptrAsinello->setIMat(instance_ptrAsinello->getIMat()* instance_di_supporto->getIMat());
	instance_ptrAsinello->InOmbrabile = true;
	instance_ptrAsinello->FaOmbra = true;
	world.add(instance_ptrAsinello);

	mesh* stalla = new mesh("models/stalla.obj", "models/");
	auto instance_ptrStalla = make_shared<instance>(stalla, m_vetro_in_aria);
	instance_ptrStalla->scale(0.75, 0.7, 0.7);
	instance_ptrStalla->scale(1.5, 1.6, 2.0);
	instance_ptrStalla->translate(-7.0f, 5.4f, 5.0f);
	instance_ptrStalla->translate(5.0f, -10.4f, -9.7f);
	instance_ptrStalla->rotate_y(17.0);
	instance_ptrStalla->InOmbrabile = true;
	instance_ptrStalla->FaOmbra = true;
	world.add(instance_ptrStalla);

	mesh* palms = new mesh("models/palmsBase.obj", "models/");
	//sinistra
	auto instance_ptrPalms = make_shared<instance>(palms, m_vetro_verde_in_aria);
	instance_ptrPalms->scale(8.5, 8.5, 8.5);
	instance_ptrPalms->translate(-16.0f, -10.0f, -9.0f);
	instance_ptrPalms->InOmbrabile = true;
	instance_ptrPalms->FaOmbra = true;
	world.add(instance_ptrPalms);
	//destra
	auto instance2_ptrPalms = make_shared<instance>(palms, m_vetro_verde_in_aria);
	instance2_ptrPalms->scale(8.5, 8.5, 8.5);
	instance2_ptrPalms->rotate_y(90.0);
	instance2_ptrPalms->translate(6.5f, -10.0f, -5.0f);
	instance2_ptrPalms->InOmbrabile = true;
	instance2_ptrPalms->FaOmbra = true;
	world.add(instance2_ptrPalms);

	mesh* threeWise = new mesh("models/threeNegros.obj", "models/");
	auto instance_ptrWise = make_shared<instance>(threeWise, m_vetro_viola_in_aria);
	float scalaRe = 1.25;
	instance_ptrWise->translate(0.00, -0.38, 0.00);
	instance_ptrWise->scale(scalaRe, scalaRe, scalaRe);
	instance_ptrWise->translate(0.00, scalaRe * 0.38 + 0.13, 0.00);
	instance_ptrWise->scale(3.5, 3.5, 3.5);
	instance_ptrWise->translate(2.5f, -8.5f, 4.0f);
	instance_ptrWise->rotate_y(0.0);
	instance_ptrWise->InOmbrabile = true;
	instance_ptrWise->FaOmbra = true;
	world.add(instance_ptrWise);

	// Sfondo
	mesh* library = new mesh("models/library.obj", "models/");
	texture* library_tex = new image_texture("models/BaseColor.jpg");
	material* library_m = new lambertian(color(1.0, 1.0, 1.0));
	library_m->texture = library_tex;
	auto library_ptr = make_shared<instance>(library, library_m);
	library_ptr->scale(3.8f, 3.8f, 3.8f);
	library_ptr->rotate_y(-90.0f);
	library_ptr->translate(28.0f, -19.0f, -18.0f);
	library_ptr->InOmbrabile = true;
	library_ptr->FaOmbra = true;
	world.add(library_ptr);

	mesh* book = new mesh("models/book.obj", "models/");
	texture* book_tex = new image_texture("models/book_tex.jpg");
	material* book_m = new lambertian(color(1.0, 1.0, 1.0));
	book_m->texture = book_tex;
	auto book_ptr = make_shared<instance>(book, book_m);
	book_ptr->translate(0.1175, -0.01, 0.00);
	book_ptr->scale(1.0f, 1.1f, 1.6f);
	book_ptr->translate(-0.1175, 0.006, 0.00);
	book_ptr->scale(15.5f, 15.5f, 15.5f);
	book_ptr->rotate_y(-270.0f);
	book_ptr->translate(38.0f, 5.75f, -21.5f);
	book_ptr->InOmbrabile = true;
	book_ptr->FaOmbra = true;
	world.add(book_ptr);
	auto book2_ptr = make_shared<instance>(book, book_m);
	book2_ptr->translate(0.1175, -0.01, 0.00);
	book2_ptr->scale(1.0f, 1.1f, 1.6f);
	book2_ptr->translate(-0.1175, 0.006, 0.00);
	book2_ptr->scale(15.5f, 15.5f, 15.5f);
	book2_ptr->rotate_y(-270.0f);
	book2_ptr->translate(37.0f, 5.75f, -21.5f);
	book2_ptr->InOmbrabile = true;
	book2_ptr->FaOmbra = true;
	world.add(book2_ptr);
	auto book3_ptr = make_shared<instance>(book, book_m);
	book3_ptr->translate(0.1175, -0.01, 0.00);
	book3_ptr->scale(1.0f, 1.1f, 1.6f);
	book3_ptr->translate(-0.1175, 0.006, 0.00);
	book3_ptr->scale(15.5f, 15.5f, 15.5f);
	book3_ptr->rotate_y(-270.0f);
	book3_ptr->translate(36.0f, 5.75f, -21.5f);
	book3_ptr->InOmbrabile = true;
	book3_ptr->FaOmbra = true;
	world.add(book3_ptr);
	auto book4_ptr = make_shared<instance>(book, book_m);
	book4_ptr->translate(0.1175, -0.01, 0.00);
	book4_ptr->scale(1.0f, 1.1f, 1.6f);
	book4_ptr->translate(-0.1175, 0.006, 0.00);
	book4_ptr->scale(15.5f, 15.5f, 15.5f);
	book4_ptr->rotate_y(-270.0f);
	book4_ptr->translate(35.0f, 5.75f, -21.5f);
	book4_ptr->InOmbrabile = true;
	book4_ptr->FaOmbra = true;
	world.add(book4_ptr);
	auto book5_ptr = make_shared<instance>(book, book_m);
	book5_ptr->translate(0.1175, -0.01, 0.00);
	book5_ptr->scale(1.0f, 1.1f, 1.6f);
	book5_ptr->translate(-0.1175, 0.006, 0.00);
	book5_ptr->scale(15.5f, 15.5f, 15.5f);
	book5_ptr->rotate_y(-270.0f);
	book5_ptr->translate(34.0f, 5.75f, -21.5f);
	book5_ptr->InOmbrabile = true;
	book5_ptr->FaOmbra = true;
	world.add(book5_ptr);
	auto book6_ptr = make_shared<instance>(book, book_m);
	book6_ptr->translate(0.1175, -0.01, 0.00);
	book6_ptr->scale(1.0f, 1.1f, 1.6f);
	book6_ptr->translate(-0.1175, 0.006, 0.00);
	book6_ptr->scale(15.5f, 15.5f, 15.5f);
	book6_ptr->rotate_y(-270.0f);
	book6_ptr->translate(33.0f, 5.75f, -21.5f);
	book6_ptr->InOmbrabile = true;
	book6_ptr->FaOmbra = true;
	world.add(book6_ptr);

	mesh* greenBook = new mesh("models/greenBook.obj", "models/");
	texture* greenBook_tex = new image_texture("models/greenBook_tex.jpg");
	material* greenBook_m = new lambertian(color(1.0, 1.0, 1.0));
	greenBook_m->texture = greenBook_tex;
	auto greenBook_ptr = make_shared<instance>(greenBook, greenBook_m);
	greenBook_ptr->scale(1.7f, 1.5f, 1.7f);
	greenBook_ptr->rotate_y(-90.0f);
	greenBook_ptr->rotate_z(180.0f);
	greenBook_ptr->translate(39.0f, 2.8f, -22.5f);
	greenBook_ptr->translate(-0.25f, 0.0f, 0.f);
	greenBook_ptr->InOmbrabile = true;
	greenBook_ptr->FaOmbra = true;
	world.add(greenBook_ptr);
	auto greenBook2_ptr = make_shared<instance>(greenBook, greenBook_m);
	greenBook2_ptr->scale(1.7f, 1.5f, 1.7f);
	greenBook2_ptr->rotate_y(-90.0f);
	greenBook2_ptr->rotate_z(180.0f);
	greenBook2_ptr->translate(38.0f, 2.8f, -22.5f);
	greenBook2_ptr->translate(-0.15f, 0.0f, 0.f);
	greenBook2_ptr->InOmbrabile = true;
	greenBook2_ptr->FaOmbra = true;
	world.add(greenBook2_ptr);
	auto greenBook3_ptr = make_shared<instance>(greenBook, greenBook_m);
	greenBook3_ptr->scale(1.7f, 1.5f, 1.7f);
	greenBook3_ptr->rotate_y(-90.0f);
	greenBook3_ptr->rotate_z(180.0f);
	greenBook3_ptr->translate(37.0f, 2.8f, -22.5f);
	greenBook3_ptr->translate(-0.05f, 0.0f, 0.f);
	greenBook3_ptr->InOmbrabile = true;
	greenBook3_ptr->FaOmbra = true;
	world.add(greenBook3_ptr);
	auto greenBook4_ptr = make_shared<instance>(greenBook, greenBook_m);
	greenBook4_ptr->scale(1.7f, 1.5f, 1.7f);
	greenBook4_ptr->rotate_y(-90.0f);
	greenBook4_ptr->rotate_z(180.0f);
	greenBook4_ptr->translate(36.0f, 2.8f, -22.5f);
	greenBook4_ptr->translate(0.05f, 0.0f, 0.f);
	greenBook4_ptr->InOmbrabile = true;
	greenBook4_ptr->FaOmbra = true;
	world.add(greenBook4_ptr);
	auto greenBook5_ptr = make_shared<instance>(greenBook, greenBook_m);
	greenBook5_ptr->scale(1.7f, 1.5f, 1.7f);
	greenBook5_ptr->rotate_y(-90.0f);
	greenBook5_ptr->rotate_z(180.0f);
	greenBook5_ptr->translate(35.0f, 2.8f, -22.5f);
	greenBook5_ptr->translate(0.15f, 0.0f, 0.f);
	greenBook5_ptr->InOmbrabile = true;
	greenBook5_ptr->FaOmbra = true;
	world.add(greenBook5_ptr);

	mesh* book2 = new mesh("models/book2.obj", "models/");
	texture* book_tex2 = new image_texture("models/book_tex2.jpg");
	material* book_m2 = new lambertian(color(1.0, 1.0, 1.0));
	book_m2->texture = book_tex2;
	auto book_ptr2 = make_shared<instance>(book2, book_m2);
	book_ptr2->scale(2.5f, 2.5f, 2.5f);
	book_ptr2->rotate_y(-270.0f);
	book_ptr2->translate(38.0f, -9.0f, -22.0f);
	book_ptr2->translate(0.34f, 0.39f, 0.0f);
	book_ptr2->InOmbrabile = true;
	book_ptr2->FaOmbra = true;
	world.add(book_ptr2);
	auto book2_ptr2 = make_shared<instance>(book2, book_m2);
	book2_ptr2->scale(2.5f, 2.5f, 2.5f);
	book2_ptr2->rotate_y(-270.0f);
	book2_ptr2->translate(37.0f, -9.0f, -22.0f);
	book2_ptr2->translate(0.34f, 0.39f, 0.0f);
	book2_ptr2->translate(0.11f, 0.0f, 0.0f);
	book2_ptr2->InOmbrabile = true;
	book2_ptr2->FaOmbra = true;
	world.add(book2_ptr2);
	auto book3_ptr2 = make_shared<instance>(book2, book_m2);
	book3_ptr2->scale(2.5f, 2.5f, 2.5f);
	book3_ptr2->rotate_y(-270.0f);
	book3_ptr2->translate(36.0f, -9.0f, -22.0f);
	book3_ptr2->translate(0.34f, 0.39f, 0.0f);
	book3_ptr2->translate(0.22f, 0.0f, 0.0f);
	book3_ptr2->InOmbrabile = true;
	book3_ptr2->FaOmbra = true;
	world.add(book3_ptr2);
	auto book4_ptr2 = make_shared<instance>(book2, book_m2);
	book4_ptr2->scale(2.5f, 2.5f, 2.5f);
	book4_ptr2->rotate_y(-270.0f);
	book4_ptr2->translate(35.0f, -9.0f, -22.0f);
	book4_ptr2->translate(0.34f, 0.39f, 0.0f);
	book4_ptr2->translate(0.33f, 0.0f, 0.0f);
	book4_ptr2->InOmbrabile = true;
	book4_ptr2->FaOmbra = true;
	world.add(book4_ptr2);
	auto book5_ptr2 = make_shared<instance>(book2, book_m2);
	book5_ptr2->scale(2.5f, 2.5f, 2.5f);
	book5_ptr2->rotate_y(-270.0f);
	book5_ptr2->translate(34.0f, -9.0f, -22.0f);
	book5_ptr2->translate(0.34f, 0.39f, 0.0f);
	book5_ptr2->translate(0.44f, 0.0f, 0.0f);
	book5_ptr2->InOmbrabile = true;
	book5_ptr2->FaOmbra = true;
	world.add(book5_ptr2);
	auto book6_ptr2 = make_shared<instance>(book2, book_m2);
	book6_ptr2->scale(2.5f, 2.5f, 2.5f);
	book6_ptr2->rotate_y(-270.0f);
	book6_ptr2->translate(33.0f, -9.0f, -22.0f);
	book6_ptr2->translate(0.34f, 0.39f, 0.0f);
	book6_ptr2->translate(0.55f, 0.0f, 0.0f);
	book6_ptr2->InOmbrabile = true;
	book6_ptr2->FaOmbra = true;
	world.add(book6_ptr2);

	mesh* bookend = new mesh("models/bookend.obj", "models/");
	texture* bookend_tex = new image_texture("models/marmoGiallo.jpg");
	lambertian_shiny* bookend_m = new lambertian_shiny(color(1.0, 1.0, 1.0), 0.3, 0.2);
	bookend_m->texture = bookend_tex;
	auto bookend_ptr = make_shared<instance>(bookend, bookend_m);
	bookend_ptr->scale(0.215f, 0.215f, 0.215f);
	bookend_ptr->rotate_y(-90.0f);
	bookend_ptr->translate(32.3f, -8.7f, -22.0f);
	bookend_ptr->translate(0.28f, 0.0f, 0.0f);
	bookend_ptr->InOmbrabile = true;
	bookend_ptr->FaOmbra = true;
	world.add(bookend_ptr);

	mesh* vase = new mesh("models/vase.obj", "models/");
	texture* vase_tex = new image_texture("models/vase_tex.jpg");
	material* vase_m = new lambertian_shiny(color(1.0, 1.0, 1.0), 0.05, 0.3);
	vase_m->texture = vase_tex;
	auto vase_ptr = make_shared<instance>(vase, vase_m);
	vase_ptr->scale(0.8f, 0.8f, 0.8f);
	vase_ptr->translate(28.0f, 5.8f, -22.0f);
	vase_ptr->InOmbrabile = true;
	vase_ptr->FaOmbra = true;
	world.add(vase_ptr);

	mesh* chair = new mesh("models/chair.obj", "models/");
	texture* chair_tex = new image_texture("models/chairTexture.jpg");
	material* chair_m = new lambertian_shiny(color(1.0, 1.0, 1.0), 0.05, 0.3);
	chair_m->texture = chair_tex;
	auto chair_ptr = make_shared<instance>(chair, chair_m);
	chair_ptr->scale(0.15f, 0.15f, 0.15f);
	chair_ptr->rotate_y(-150.0f);
	chair_ptr->translate(35.0f, -20.5f, 5.0f);
	chair_ptr->InOmbrabile = true;
	chair_ptr->FaOmbra = true;
	world.add(chair_ptr);

	mesh* tv = new mesh("models/TV.obj", "models/");
	texture* tv_tex = new image_texture("models/TV_tex.jpg");
	material* tv_m = new lambertian(color(1.0, 1.0, 1.0));
	tv_m->texture = tv_tex;
	auto tv_ptr = make_shared<instance>(tv, tv_m);
	tv_ptr->rotate_y(-20.0f);
	tv_ptr->scale(12.0, 12.0, 12.0);
	tv_ptr->translate(-49.0f, -8.7f, -34.0f);
	tv_ptr->rotate_y(20.0f);
	tv_ptr->translate(80.0f, 0.0f, -5.0f);
	tv_ptr->InOmbrabile = true;
	tv_ptr->FaOmbra = true;
	world.add(tv_ptr);

	mesh* sideTableNativity = new mesh("models/tavolinoPresepe.obj", "models/");
	texture* sideTableNativity_tex = new image_texture("models/BaseColor.jpg");
	material* sideTableNativity_m = new lambertian(color(1.0, 1.0, 1.0));
	sideTableNativity_m->texture = sideTableNativity_tex;
	auto sideTableNativity_ptr = make_shared<instance>(sideTableNativity, sideTableNativity_m);
	sideTableNativity_ptr->scale(1.5, 1.6, 2.0);
	sideTableNativity_ptr->scale(2.2, 2.2, 2.2);
	sideTableNativity_ptr->translate(-6.0f, -16.1f, 3.0f);
	sideTableNativity_ptr->translate(-7.0f, 1.88f, -2.0f);
	sideTableNativity_ptr->InOmbrabile = true;
	sideTableNativity_ptr->FaOmbra = true;
	world.add(sideTableNativity_ptr);

	material* red_wall = new lambertian(color(0.75, 0.05, 0.05));
	red_wall->texture = NULL;
	material* white_wall = new lambertian(color(0.93, 0.93, 0.93));
	white_wall->texture = NULL;
	material* green_wall = new lambertian(color(0.12, 0.75, 0.15));
	green_wall->texture = NULL;

	mesh* room = new mesh("models/room.obj", "models/");
	texture* room_tex = new image_texture("models/panna.jpg");
	material* room_m = new lambertian(color(1.0, 1.0, 1.0));
	room_m->texture = room_tex;
	auto room_ptr = make_shared<instance>(room, room_m);
	room_ptr->scale(1.9, 3.0, 3.0);
	room_ptr->translate(20.0f, 0.0f, 0.0f);
	room_ptr->translate(-6.0f, -14.5f, 15.0f);
	room_ptr->InOmbrabile = true;
	room_ptr->FaOmbra = true;
	//world.add(room_ptr);

	// Right wall
	point3 v1 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, -13.44689));
	point3 v2 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, 26.595016));
	point3 v3 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, 26.595016));
	point3 v4 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, -13.44689));
	quadrilatero* rectangle_model3 = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr3 = make_shared<instance>(rectangle_model3, green_wall);
	world.add(rectangle_ptr3);

	// Left wall
	v1 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, 26.595016));
	v2 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, -13.44689));
	v3 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, -13.44689));
	v4 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, 26.595016));
	quadrilatero* rectangle_model4 = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr4 = make_shared<instance>(rectangle_model4, red_wall);
	world.add(rectangle_ptr4);

	// Ceiling
	v1 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, -13.44689));
	v2 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, -13.44689));
	v3 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, 26.595016));
	v4 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, 26.595016));
	quadrilatero* rectangle_model5 = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr5 = make_shared<instance>(rectangle_model5, white_wall);
	world.add(rectangle_ptr5);

	// Floor
	v1 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, 26.595016));
	v2 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, 26.595016));
	v3 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, -13.44689));
	v4 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, -13.44689));
	quadrilatero* rectangle_model6 = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr6 = make_shared<instance>(rectangle_model6, white_wall);
	world.add(rectangle_ptr6);

	// Front wall
	v1 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, -13.44689));
	v2 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, -13.44689));
	v3 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, -13.44689));
	v4 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, -13.44689));
	quadrilatero* rectangle_model7 = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr7 = make_shared<instance>(rectangle_model7, white_wall);
	world.add(rectangle_ptr7);

	// Back wall
	v1 = multiply(room_ptr->getCMat(), point3(18.0832, -1.82525, 15.0));
	v2 = multiply(room_ptr->getCMat(), point3(-21.8837, -1.82525, 15.0));
	v3 = multiply(room_ptr->getCMat(), point3(-21.8837, 13.01412, 15.0));
	v4 = multiply(room_ptr->getCMat(), point3(18.0832, 13.01412, 15.0));
	quadrilatero* rectangle_model = new quadrilatero(v1, v2, v3, v4);
	auto rectangle_ptr = make_shared<instance>(rectangle_model, white_wall);
	world.add(rectangle_ptr);

	mesh* painting = new mesh("models/painting.obj", "models/");
	painting->loadImgNorm("models/cadre_lowpoly_blinn2_Normal.jpg");
	texture* painting_tex = new image_texture("models/painting.jpg");
	material* painting_m = new lambertian(color(1.0, 1.0, 1.0));
	painting_m->texture = painting_tex;
	auto painting_ptr = make_shared<instance>(painting, painting_m);
	painting_ptr->rotate_x(90.0f);
	painting_ptr->rotate_z(90.0f);
	painting_ptr->scale(10.0, 10.0, 10.0);
	point3 pos_rel(-9.00, 7.00, -13.48);
	painting_ptr->translate(multiply(room_ptr->getCMat(), pos_rel));
	painting_ptr->InOmbrabile = true;
	painting_ptr->FaOmbra = true;
	world.add(painting_ptr);

	mesh* clock = new mesh("models/WallClock_LP.obj", "models/");
	clock->loadImgNorm("models/WallClock_LP_Clock_Normal.jpg");
	texture* clock_tex = new image_texture("models/WallClock_LP_Clock_Specular.jpg");
	material* clock_m = new lambertian(color(1.0, 1.0, 1.0));
	clock_m->texture = clock_tex;
	auto clock_ptr = make_shared<instance>(clock, clock_m);
	clock_ptr->translate(0.00, 0.00, -73.75);
	clock_ptr->scale(0.025, 0.025, 0.05);
	clock_ptr->rotate_y(90.0);
	clock_ptr->translate(multiply(room_ptr->getCMat(), point3(-21.89, 8.00, -7.00)));
	clock_ptr->InOmbrabile = true;
	clock_ptr->FaOmbra = true;
	world.add(clock_ptr);

	mesh* clock_cover = new mesh("models/WallClock_LP_plastica.obj", "models/");
	clock->loadImgNorm("models/WallClock_LP_Clock_Normal.jpg");
	auto clock_cover_ptr = make_shared<instance>(clock_cover, m_plastica_in_aria);
	clock_cover_ptr->setCMat(clock_ptr->getCMat());
	clock_cover_ptr->setIMat(clock_ptr->getIMat());
	clock_cover_ptr->InOmbrabile = true;
	clock_cover_ptr->FaOmbra = false;
	world.add(clock_cover_ptr);

	auto clock_cover_ptr2 = make_shared<instance>(clock_cover, m_aria_in_plastica);
	clock_cover_ptr2->translate(0.00, 0.00, -5.70);
	clock_cover_ptr2->setCMat(clock_ptr->getCMat()* clock_cover_ptr2->getCMat());
	clock_cover_ptr2->setIMat(clock_cover_ptr2->getIMat()* clock_ptr->getIMat());
	clock_cover_ptr2->InOmbrabile = true;
	clock_cover_ptr2->FaOmbra = false;
	world.add(clock_cover_ptr2);

	mesh* chandelier = new mesh("models/chandelier.obj", "models/");
	auto instance_ptrChandelier = make_shared<instance>(chandelier, m_vetroS_in_aria);
	instance_ptrChandelier->scale(29.0, 29.0, 29.0);
	instance_ptrChandelier->translate(0.5f, 10.0f, -14.5f);
	instance_ptrChandelier->translate(0.0f, 1.08f, 0.0f);
	instance_ptrChandelier->InOmbrabile = true;
	instance_ptrChandelier->FaOmbra = false;
	world.add(instance_ptrChandelier);

	mesh* wallLamp2 = new mesh("models/lampadaMuro2.obj", "models/");
	auto wallLamp_ptr2 = make_shared<instance>(wallLamp2, m_vetro_in_aria);
	wallLamp_ptr2->scale(3.8, 3.8, 3.8);
	wallLamp_ptr2->rotate_y(-90.0f);
	wallLamp_ptr2->translate(50.0f, 0.0f, -10.0f);
	wallLamp_ptr2->InOmbrabile = true;
	wallLamp_ptr2->FaOmbra = false;
	world.add(wallLamp_ptr2);

	//Vaso con fiore
	// iperboloide per la superfice esterna
	float h_min = -1.0, h_max = 1.0;
	iperb1faldaStandard* vasoFiore1 = new iperb1faldaStandard(h_min, h_max);
	auto iperb1_ptr = make_shared<instance>(vasoFiore1, m_vetro_in_aria);
	iperb1_ptr->translate(0.0, -h_min, 0.0);
	iperb1_ptr->scale(1.5, 4.0, 1.5);
	iperb1_ptr->scale(0.5, 0.5, 0.5);
	iperb1_ptr->translate(multiply(library_ptr->getCMat(), point3(-0.85, 5.23, 2.10)));
	world.add(iperb1_ptr);

	// iperboloide per la superfice interna
	float spessore = 0.05;
	float h_min2 = h_min + spessore * 2.0 / 3.0, h_water = h_max / 2.0, h_max2 = h_max;

	iperb1faldaStandard* vasoFioreInt1 = new iperb1faldaStandard(h_min2, h_water);
	vasoFioreInt1->normEst = false;
	auto iperb2_ptr1 = make_shared<instance>(vasoFioreInt1, m_vetro_in_acqua);
	iperb2_ptr1->scale(1.0 - spessore, 1.0, 1.0 - spessore);
	iperb2_ptr1->setCMat(iperb1_ptr->getCMat()* iperb2_ptr1->getCMat());
	iperb2_ptr1->setIMat(iperb2_ptr1->getIMat()* iperb1_ptr->getIMat());
	world.add(iperb2_ptr1);

	iperb1faldaStandard* vasoFioreInt2 = new iperb1faldaStandard(h_water, h_max2);
	vasoFioreInt2->normEst = false;
	auto iperb2_ptr2 = make_shared<instance>(vasoFioreInt2, m_vetro_in_aria);
	iperb2_ptr2->setCMat(iperb2_ptr1->getCMat());
	iperb2_ptr2->setIMat(iperb2_ptr1->getIMat());
	world.add(iperb2_ptr2);

	discoStandard* acqua_aria = new discoStandard();
	acqua_aria->normAlto = true;
	auto disc0_ptr = make_shared<instance>(acqua_aria, m_acqua_in_aria);
	disc0_ptr->scale(sqrt(1.0 + h_water * h_water), 1.0, sqrt(1.0 + h_water * h_water));
	disc0_ptr->translate(0.0, h_water, 0.0);
	disc0_ptr->setCMat(iperb2_ptr1->getCMat()* disc0_ptr->getCMat());
	disc0_ptr->setIMat(disc0_ptr->getIMat()* iperb2_ptr1->getIMat());
	world.add(disc0_ptr);

	coronaStandard* vasoFiore2 = new coronaStandard(1.0 - spessore);
	vasoFiore2->normAlto = true; // indca la direzione in cui c'è l'aria
	auto disc1_ptr = make_shared<instance>(vasoFiore2, m_vetro_in_aria);
	disc1_ptr->scale(sqrt(1.0 + h_max * h_max), 1.0, sqrt(1.0 + h_max * h_max));
	disc1_ptr->translate(0.0, h_max, 0.0);
	disc1_ptr->setCMat(iperb1_ptr->getCMat()* disc1_ptr->getCMat());
	disc1_ptr->setIMat(disc1_ptr->getIMat()* iperb1_ptr->getIMat());
	world.add(disc1_ptr);

	discoStandard* vasoFiore3 = new discoStandard();
	vasoFiore3->normAlto = false;
	auto disc2_ptr = make_shared<instance>(vasoFiore3, m_vetro_in_aria);
	disc2_ptr->scale(sqrt(1.0 + h_min * h_min), 1.0, sqrt(1.0 + h_min * h_min));
	disc2_ptr->translate(0.0, h_min, 0.0);
	disc2_ptr->setCMat(iperb1_ptr->getCMat()* disc2_ptr->getCMat());
	disc2_ptr->setIMat(disc2_ptr->getIMat()* iperb1_ptr->getIMat());
	world.add(disc2_ptr);

	discoStandard* vasoFiore4 = new discoStandard();
	vasoFiore3->normAlto = true;
	auto disc3_ptr = make_shared<instance>(vasoFiore4, m_vetro_in_acqua);
	disc3_ptr->scale(sqrt(1.0 + h_min2 * h_min2), 1.0, sqrt(1.0 + h_min2 * h_min2));
	disc3_ptr->translate(0.0, h_min2, 0.0);
	disc3_ptr->setCMat(iperb2_ptr1->getCMat()* disc3_ptr->getCMat());
	disc3_ptr->setIMat(disc3_ptr->getIMat()* iperb2_ptr1->getIMat());
	world.add(disc3_ptr);

	//Tulipano
	mesh* flower1 = new mesh("models/tulip_flower_stalk.obj", "models/");
	texture* flower1_tex = new image_texture("models/tulip_flower_stalk_Diffuse.jpg");
	material* flower1_m = new lambertian(color(1.0, 1.0, 1.0));
	flower1_m->texture = flower1_tex;
	auto flower1_ptr = make_shared<instance>(flower1, flower1_m);
	flower1_ptr->rotate_x(-90.0);
	flower1_ptr->scale(0.4, 0.2, 0.4);
	flower1_ptr->translate(0.0, h_min2 + 0.001, 0.0);
	flower1_ptr->rotate_z(20.0);
	flower1_ptr->setCMat(iperb1_ptr->getCMat()* flower1_ptr->getCMat());
	flower1_ptr->setIMat(flower1_ptr->getIMat()* iperb1_ptr->getIMat());
	flower1_ptr->InOmbrabile = true;
	flower1_ptr->FaOmbra = true;
	world.add(flower1_ptr);

	mesh* flower2 = new mesh("models/tulip_flower_leaf.obj", "models/");
	texture* flower2_tex = new image_texture("models/tulip_flower_leaf_Diffuse.jpg");
	material* flower2_m = new lambertian_shiny(color(1.0, 1.0, 1.0), 0.15, 0.2);
	flower2_m->texture = flower2_tex;
	auto flower2_ptr = make_shared<instance>(flower2, flower2_m);
	flower2_ptr->setCMat(flower1_ptr->getCMat());
	flower2_ptr->setIMat(flower1_ptr->getIMat());
	flower2_ptr->InOmbrabile = true;
	flower2_ptr->FaOmbra = true;
	world.add(flower2_ptr);

	mesh* flower3 = new mesh("models/tulip_flower_petal.obj", "models/");
	texture* flower3_tex = new image_texture("models/tulip_flower_petal_Diffuse.jpg");
	material* flower3_m = new lambertian(color(1.0, 1.0, 1.0));
	flower3_m->texture = flower3_tex;
	auto flower3_ptr = make_shared<instance>(flower3, flower3_m);
	flower3_ptr->setCMat(flower1_ptr->getCMat());
	flower3_ptr->setIMat(flower1_ptr->getIMat());
	flower3_ptr->InOmbrabile = true;
	flower3_ptr->FaOmbra = true;
	world.add(flower3_ptr);


	// Stella Cometa
	material* light_stella = new area_light();
	light_stella->texture = new constant_texture(5 * color(252.0 / 255.0, 186.0 / 255.0, 3.0 / 255.0));
	mesh* stella = new mesh("models/Stella_Cometa.obj", "models/");
	auto stella_ptr = make_shared<instance>(stella, light_stella);
	stella_ptr->translate(-150.00, -65.00, 0.00);
	stella_ptr->scale(0.01, 0.01, 0.01);
	stella_ptr->rotate_y(-16.0);
	stella_ptr->rotate_y(-30.0);
	stella_ptr->translate(multiply(instance_ptrStalla->getCMat(), point3(2.50, -1.00, -2.42)));
	stella_ptr->translate(1.00, -0.50, 2.00);
	world.add(stella_ptr);

	// pannello led
	material* light_pannello = new area_light();
	light_pannello->texture = new constant_texture(color(10.0, 10.0, 10.0));
	point3 w1 = multiply(instance_ptrStalla->getCMat(), point3(-1.276964, - 0.146495, - 1.562272));//26
	point3 w2 = multiply(instance_ptrStalla->getCMat(), point3(1.920536, - 0.105731, - 0.627021));//22
	point3 w3 = multiply(instance_ptrStalla->getCMat(), point3(1.279494, - 0.095292, 1.564164));
	point3 w4 = multiply(instance_ptrStalla->getCMat(), point3(-1.918006, - 0.136056, 0.628913));//27
	quadrilatero* rectangle_model8 = new quadrilatero(w1, w2, w3, w4);
	auto rectangle_ptr8 = make_shared<instance>(rectangle_model8, light_pannello);
	world.add(rectangle_ptr8);

	// candele nel sistema relativo del candelabro
	//  0, 10; -0, 22;  0, 175
	// -0, 10; -0, 22;  0, 175
	// -0, 10; -0, 22; -0, 175
	//  0, 10; -0, 22; -0, 175
	//  0, 205; -0,22;  0, 00
	// -0,20;   -0,22;   0,00
	material* light_chandelier = new area_light();
	light_chandelier->texture = new constant_texture(color(12, 12, 12));

	auto light_position2_1 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_1->translate(multiply(instance_ptrChandelier->getCMat(), point3(0.105, 0.22, 0.175)));
	world.add(light_position2_1);

	auto light_position2_2 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_2->translate(multiply(instance_ptrChandelier->getCMat(), point3(-0.105, 0.22, 0.175)));
	world.add(light_position2_2);

	auto light_position2_3 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_3->translate(multiply(instance_ptrChandelier->getCMat(), point3(-0.105, 0.22, -0.175)));
	world.add(light_position2_3);

	auto light_position2_4 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_4->translate(multiply(instance_ptrChandelier->getCMat(), point3(0.105, 0.22, -0.175)));
	world.add(light_position2_4);

	auto light_position2_5 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_5->translate(multiply(instance_ptrChandelier->getCMat(), point3(-0.205, 0.22, 0.0)));
	world.add(light_position2_5);

	auto light_position2_6 = make_shared<instance>(sphere_model, light_chandelier);
	light_position2_6->translate(multiply(instance_ptrChandelier->getCMat(), point3(0.2, 0.22, 0.0)));
	world.add(light_position2_6);


	// luci esterne: ai due angoli superiori della beck wall metto due luci sferiche
	material* light_external = new area_light();
	light_external->texture = new constant_texture(color(20, 20, 20));
	auto light_position3_1 = make_shared<instance>(sphere_model, light_external);
	light_position3_1->scale(6.5, 6.5, 6.5);
	light_position3_1->translate(v3);
	world.add(light_position3_1);

	auto light_position3_2 = make_shared<instance>(sphere_model, light_external);
	light_position3_2->scale(6.5, 6.5, 6.5);
	light_position3_2->translate(v4);
	world.add(light_position3_2);


	camera cam;
	// vista panoramica
	cam.lookfrom = point3(0.0, 0.0, 55.0);
	cam.lookat = point3(10.0f, 0.0f, -0.15f);
	cam.vup = point3(0.0, 1.0, 0.0);
	cam.vfov = 45;

	// vista presepe
	//cam.lookfrom = point3(-12.0, -7.0, 30.0);
	//cam.lookat = point3(-5.0f, -7.0f, -0.15f);
	//cam.vup = point3(0.0, 1.0, 0.0);
	//cam.vfov = 30;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 1280;
	cam.samples_per_pixel = 10;

	cam.initialize();

	if (init(cam.image_width, cam.image_height) == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	cout << "Image Resolution: " << cam.image_width << "x" << cam.image_height << "\n255\n";

	time_t start, end;
	double dif;

	// Creare solo una immagine
	/*
	*/
	time(&start);
	cam.parallel_render(world, worldlight);
	time(&end);
	dif = difftime(end, start);
	cout << "Rendering time: " << dif << "\n";

	// Creare immagini a pennellate
	/*
	srand(time(0)); // per rendere le pennellate indipendenti
	
	time(&start);
	cam.parallel_render(world, worldlight); // 1645
	time(&end);
	dif = difftime(end, start);
	cout << "Rendering time: " << dif << "\n";
	saveScreenshotBMP("screenshotP_30_250_Luminoso.bmp");  // 5004 per 250

	time(&start);
	cam.parallel_renderReUseSemple(world, worldlight, "screenshotP_30_2000_Luminoso.bmp", 2000, 2250);
	time(&end);
	dif = difftime(end, start);
	cout << "Rendering time: " << dif << "\n";
	saveScreenshotBMP("screenshotP_30_2250_Luminoso.bmp"); // 4958

	time(&start);
	cam.parallel_renderReUseSemple(world, worldlight, "screenshotP_30_2250_Luminoso.bmp", 2250, 2500);
	time(&end);
	dif = difftime(end, start);
	cout << "Rendering time: " << dif << "\n";
	saveScreenshotBMP("screenshotP_30_2500_Luminoso.bmp"); // 5026

	time(&start);
	cam.parallel_renderReUseSemple(world, worldlight, "screenshotP_30_2500_Luminoso.bmp", 2500, 2750);
	time(&end);
	dif = difftime(end, start);
	cout << "Rendering time: " << dif << "\n";
	saveScreenshotBMP("screenshotP_30_2750_Luminoso.bmp"); // 5086
	*/

	// differenza immagini
	/*
	SDL_Surface* surface1;
	SDL_Surface* surface2;
	surface1 = loadTexture("ImmaginiPresepe/screenshotP_30_5000_Luminoso.jpg", cam.image_width, cam.image_height);
	surface2 = loadTexture("ImmaginiPresepe/screenshotP_30_4000_Luminoso.jpg", cam.image_width, cam.image_height);
	//surface1 = loadTexture("Immagini panoramiche/screenshot_25_4000_Luminoso.bmp", cam.image_width, cam.image_height);
	//surface2 = loadTexture("Immagini panoramiche/screenshot_25_5200_Luminoso.bmp", cam.image_width, cam.image_height);
	// The mean square error
	vec3 mse(0.0, 0.0, 0.0);
	float max_diff = 0;
	for (int j = 0; j < cam.image_height; j++) {
		for (int i = 0; i < cam.image_width; i++) {
			Uint32 value1 = getpixel(surface1, i, j);

			float red1 = float((value1 >> 16) & 0xff) / 255.0f;
			float green1 = float((value1 >> 8) & 0xff) / 255.0f;
			float blue1 = float(value1 & 0xff) / 255.0f;

			Uint32 value2 = getpixel(surface2, i, j);

			float red2 = float((value2 >> 16) & 0xff) / 255.0f;
			float green2 = float((value2 >> 8) & 0xff) / 255.0f;
			float blue2 = float(value2 & 0xff) / 255.0f;

			vec3 differenze(abs(red1 - red2), abs(green1 - green2), abs(blue1 - blue2));

			setColor(differenze[0], differenze[1], differenze[2]);
			setPixel(i, j);

			mse += differenze*differenze;

			float norma = differenze.length();
			if (norma > max_diff)
				max_diff = norma;
		}
	}
	SDL_RenderPresent(renderer);
	//saveScreenshotBMP("screenshot_differenza_25_5200-4000.bmp");

	mse = mse / (cam.image_width * cam.image_height);
	cout << "mean square error: (" << mse[0] << ", " << mse[1] << ", " << mse[2] << ")\n";
	cout << "differenza massima: " << max_diff << endl;

	// tra screenshot_25_5200_Luminoso e screenshot_25_4000_Luminoso:
	// mean square error: (0.000109866, 0.000103728, 7.68468e-05)
	// differenza massima: 0.14762
	// la convergenza inizia a stagnare => mi fermo

	// tra screenshot_25_5200_Luminoso e screenshot_25_1250_Luminoso:
	// mean square error : (0.000937796, 0.000881924, 0.000652352)
	// differenza massima : 0.487175

	// tra screenshotP_30_5000_Luminoso e screenshotP_30_4000_Luminoso:
	// mean square error : (0.000222746, 0.000225906, 0.000236698)
	// differenza massima : 0.271694
	// la convergenza inizia a stagnare => mi fermo
	*/

	// Creare immagini a fascie
	/*
	cam.parallel_render(world, worldlight, NULL, 0, cam.image_height / 5, true);
	saveScreenshotBMP("screenshot_primo_quinto.bmp");
	time(&end);
	dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";

	cam.parallel_render(world, worldlight, "screenshot_primo_quinto.bmp", cam.image_height / 5 + 1, 2 * cam.image_height / 5, true);
	saveScreenshotBMP("screenshot_secondo_quinto.bmp");
	time(&end);
	dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";

	cam.parallel_render(world, worldlight, "screenshot_secondo_quinto.bmp", 2 * cam.image_height / 5 + 1, 3 * cam.image_height / 5, true);
	saveScreenshotBMP("screenshot_terzo_quinto.bmp");
	time(&end);
	dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";
	// cam.image_height = 720
	// 2 * cam.image_height / 5 + 1 = 289
	// 3 * cam.image_height / 5 = 432
	// righe computate: 144
	// tempo medio di una riga: 210.6 s
	// righe all'ora: 60*60/210.6 = 17

	cam.parallel_render(world, worldlight, "screenshot_quarto_quinto0.bmp", 3 * cam.image_height / 5 + 1, 4 * cam.image_height / 5, true);
	saveScreenshotBMP("screenshot_quarto_quinto2.bmp");
	time(&end);
	dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";

	cam.parallel_render(world, worldlight, "screenshot_quarto_quinto2.bmp", 4 * cam.image_height / 5 + 1, cam.image_height - 1, true);
	saveScreenshotBMP("screenshot_35_10000.bmp");
	time(&end);
	dif = difftime(end, start);
	cout << "\n" << "Rendering time: " << dif << "\n";
	*/


	SDL_Event event;
	bool quit = false;

	/* Poll for events */
	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
				// cases for other keypresses

			case SDLK_s:
				saveScreenshotBMP("screenshot.bmp");
				cout << "Screenshot saved!" << endl;
				break;
			}
		}
	}

	close();
	return 0;
}