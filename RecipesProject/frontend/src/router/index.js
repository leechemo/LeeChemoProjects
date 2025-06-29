import { createRouter, createWebHistory } from 'vue-router';
import HomePage from "../pages/HomePage.vue";
import NotFound from "../pages/NotFoundPage.vue";
import store from "../store";
import RecipeViewPage from '../pages/RecipeViewPage.vue';


const routes = [
  {
    path: "/",
    name: "home",
    component: HomePage,
    meta: {
      title: "דף הבית - Vue Recipes",
      requiresAuth: false
    }
  },
  {
    path: "/home",
    name: "home-alias",
    component: HomePage,
    meta: {
      title: "דף הבית - Vue Recipes",
      requiresAuth: false
    }
  },
  {
    path: "/register",
    name: "register",
    component: () => import("../pages/RegisterPage.vue"),
    meta: {
      title: "הרשמה - Vue Recipes",
      requiresAuth: false,
      guestOnly: true // רק למשתמשים לא מחוברים
    }
  },
  {
    path: "/login",
    name: "login",
    component: () => import("../pages/LoginPage.vue"),
    meta: {
      title: "התחברות - Vue Recipes",
      requiresAuth: false,
      guestOnly: true // רק למשתמשים לא מחוברים
    }
  },
  {
    path: "/search",
    name: "search",
    component: () => import("../pages/SearchPage.vue"),
    meta: {
      title: "חיפוש מתכונים - Vue Recipes",
      requiresAuth: false
    }
  },
  {
    path: "/recipe/:recipeID",
    name: "recipe",
    component: () => import("../pages/RecipeViewPage.vue"),
    meta: {
      title: "מתכון - Vue Recipes",
      requiresAuth: false
    }
  },
  {
    path: "/about",
    name: "about",
    component: () => import("../pages/AboutPage.vue"),
    meta: {
      title: "אודות - Vue Recipes",
      requiresAuth: false
    }
  },
  {
    path: "/profile",
    name: "profile",
    component: () => import("../pages/ProfilePage.vue"),
    meta: {
      title: "פרופיל - Vue Recipes",
      requiresAuth: true // רק למשתמשים מחוברים
    }
  },
  {
    path: "/favorites",
    name: "favorites",
    component: () => import("../pages/FavoritesPage.vue"),
    meta: {
      title: "מועדפים - Vue Recipes",
      requiresAuth: true // רק למשתמשים מחוברים
    }
  },
  {
    path: '/user/my-recipes/:recipeID',
    name: 'MyRecipeView',
    component: RecipeViewPage
  },
  {
    path: '/user/family-recipes/:recipeID',
    name: 'FamilyRecipeView',
    component: RecipeViewPage
  },
  {
  path: '/recipes/:recipeID/prepare',
  name: 'PrepareRecipe',
  component: () => import("../pages/PrepareRecipePage.vue")
  },
  {
    path: '/meal-plan',
    name: 'MealPlan',
    component: () => import("../pages/MealPlanPage.vue"),
    meta: {
      title: "Meal Plan - Vue Recipes",
      requiresAuth: true
    }
  },
  {
    path: '/user/my-recipes',
    name: 'my-recipes',
    component: () => import('@/pages/MyRecipesPage.vue')
  },
  {
    path: '/user/family-recipes',
    name: 'family-recipes',
    component: () => import('@/pages/FamilyRecipesPage.vue')
  },
  {
    path: '/recipes/fullview/:recipeID',
    name: 'FullRecipeView',
    component: RecipeViewPage
  },
  {
    path: '/user/my-recipes/:recipeID/prepare',
    name: 'PrepareRecipePersonal',
    component: () => import("../pages/PrepareRecipePage.vue"),
    meta: { requiresAuth: true }
  },
  {
    path: '/user/family-recipes/:recipeID/prepare',
    name: 'PrepareRecipeFamily',
    component: () => import("../pages/PrepareRecipePage.vue"),
    meta: { requiresAuth: true }
  },
  {
    path: "/:catchAll(.*)",
    name: "notFound",
    component: NotFound,
    meta: {
      title: "404 - עמוד לא נמצא",
      requiresAuth: false
    }
  }
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

// Global Before Each Guard
router.beforeEach((to, from, next) => {
  if (to.meta && to.meta.title) {
    document.title = to.meta.title;
  }

  const isAuthenticated = store.username || localStorage.getItem('username');

  if (to.meta.guestOnly && isAuthenticated) {
    next({ name: 'home' });
    return;
  }

  if (to.meta.requiresAuth && !isAuthenticated) {
    localStorage.setItem('redirectAfterLogin', to.fullPath);
    if (window.toast) {
      window.toast('גישה מוגבלת', 'עליך להתחבר כדי לגשת לעמוד זה', 'warning');
    }
    next({ name: 'login' });
    return;
  }

  next();
});

router.afterEach((to) => {
  window.scrollTo(0, 0);  
  if (to.name !== 'notFound') {
    console.log(`Navigated to: ${to.name}`);
  }
});

export default router;
