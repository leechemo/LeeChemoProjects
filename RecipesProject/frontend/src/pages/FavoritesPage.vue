<template>
  <div class="container-fluid py-4">
    <div class="row justify-content-center">
      <div class="col-lg-10">
        <!-- Page Header -->
        <div class="text-center mb-4">
          <h1 class="display-4 text-primary">
            <i class="bi bi-heart-fill"></i> My Favorite Recipes
          </h1>
          <p class="lead text-muted">View and enjoy your saved favorite recipes</p>
        </div>

        <!-- Loading State -->
        <div v-if="loading" class="text-center py-5">
          <div class="spinner-border text-primary mb-3" role="status">
            <span class="visually-hidden">Loading...</span>
          </div>
          <p class="text-muted">Loading favorite recipes...</p>
        </div>

        <!-- Favorites List -->
        <div v-else-if="recipes.length > 0" class="row">
          <div 
            v-for="recipe in recipes" 
            :key="recipe.recipeID" 
            class="col-md-6 col-lg-4 mb-4"
          >
            <router-link 
              :to="getRecipeRoute(recipe)" 
              class="text-decoration-none text-dark"
            >
              <RecipePreview :recipe="recipe" />
            </router-link>
          </div>
        </div>

        <!-- Empty State -->
        <div v-else class="text-center py-5">
          <div class="card shadow-sm">
            <div class="card-body py-5">
              <i class="bi bi-heart display-1 text-muted mb-3"></i>
              <h3 class="text-muted mb-3">You have no favorite recipes yet</h3>
              <p class="text-muted mb-4">
                Start browsing recipes and add them to your favorites to see them here.
              </p>
              <router-link to="/search" class="btn btn-primary btn-lg">
                <i class="bi bi-search me-2"></i> Search Recipes
              </router-link>
            </div>
          </div>
        </div>

      </div>
    </div>
  </div>
</template>

<script>
import RecipePreview from "@/components/RecipePreview.vue";
import axios from "axios";
import store from "@/store.js";

export default {
  name: "FavoritesPage",
  components: { RecipePreview },
  data() {
    return {
      recipes: [],
      loading: false,
    };
  },
  async created() {
    this.loading = true;
    try {
      const response = await axios.get(`${store.server_domain}/user/favorites`, { withCredentials: true });
      this.recipes = response.data;
    } catch (err) {
      console.error("Error loading favorites:", err);
    } finally {
      this.loading = false;
    }
  },
  methods: {
    getRecipeRoute(recipe) {
      const recipeID = recipe.recipeID;
      const isFamily = recipe.isFamily === true || recipe.isFamily === 1;

      if (recipeID.startsWith('RU')) {
        if (isFamily) {
          return { name: 'FamilyRecipeView', params: { recipeID: recipeID } };
        } else {
          return { name: 'MyRecipeView', params: { recipeID: recipeID } };
        }
      } else {
        return { name: 'FullRecipeView', params: { recipeID: recipeID } };
      }
    }

  }
};
</script>

<style scoped>
.recipe-card {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: none;
  border-radius: 15px;
}

.recipe-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.15) !important;
}

.recipe-image {
  height: 200px;
  object-fit: cover;
  border-radius: 15px 15px 0 0;
}

.recipe-title {
  font-size: 1.1rem;
  font-weight: 600;
  color: #333;
  margin-bottom: 0.5rem;
  line-height: 1.3;
}

.recipe-meta {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}

.recipe-description {
  color: #666;
  line-height: 1.5;
  display: -webkit-box;
  -webkit-line-clamp: 2;
  -webkit-box-orient: vertical;
  overflow: hidden;
  font-size: 0.9rem;
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}
</style>
