<template>
  <div class="container-fluid py-4">
    <div class="row justify-content-center">
      <div class="col-lg-10">
        <!-- Page Header -->
        <div class="text-center mb-4">
          <h1 class="display-4 text-primary">
            <i class="bi bi-people"></i> My Family Recipes
          </h1>
          <p class="lead text-muted">Family recipes passed down through generations</p>
        </div>

        <!-- Create Recipe Button -->
        <div class="text-center mb-4">
          <button @click="showCreateModal = true" class="btn btn-primary btn-lg">
            <i class="bi bi-plus-circle me-2"></i> Add New Family Recipe
          </button>
        </div>

        <!-- Loading State -->
        <div v-if="loading" class="text-center py-5">
          <div class="spinner-border text-primary mb-3" role="status">
            <span class="visually-hidden">Loading...</span>
          </div>
          <p class="text-muted">Loading family recipes...</p>
        </div>

        <!-- Family Recipes Grid -->
        <div v-else-if="recipes.length > 0" class="row">
          <div 
            v-for="recipe in recipes" 
            :key="recipe.recipeID" 
            class="col-md-6 col-lg-4 mb-4"
          >
            
            <router-link 
              :to="{ name: 'FamilyRecipeView', params: { recipeID: recipe.recipeID } }"
              class="card h-100 family-recipe-card text-decoration-none text-dark"
            >
              <div class="recipe-image-container position-relative">
                <img 
                  :src="recipe.image" 
                  :alt="recipe.title"
                  class="card-img-top recipe-image"
                />

                <!--Family member badge-->
                <div class="position-absolute top-0 start-0 m-2">
                  <span class="badge bg-warning text-dark">
                    <i class="bi bi-person-heart"></i> {{ recipe.familyMember }}
                  </span>
                </div>

                <!--Occasion member badge-->
                <div class="position-absolute top-0 end-0 m-2">
                  <span class="badge bg-info">
                    <i class="bi bi-calendar-event"></i> {{ recipe.occasion }}
                  </span>
                </div>
              </div>
              
              <div class="card-body">
                <h5 class="card-title">{{ recipe.title }}</h5>
                
                <!--Recipe Meta-->
                <div class="recipe-meta mb-2">
                  <span class="badge bg-primary me-1">
                    <i class="bi bi-clock"></i> {{ recipe.readyInMinutes }} min
                  </span>
                  <span class="badge bg-success">
                    <i class="bi bi-people"></i> {{ recipe.servings }} servings
                  </span>
                </div>

                <!--Dietary Info-->
                <div class="dietary-info mb-3">
                  <span v-if="recipe.vegetarian" class="badge bg-info me-1">
                    <i class="bi bi-flower1"></i> Vegetarian
                  </span>
                  <span v-if="recipe.glutenFree" class="badge bg-warning">
                    <i class="bi bi-shield-check"></i> Gluten-Free
                  </span>
                </div>
              </div>
            </router-link>
          </div>
        </div>

        <!-- Empty State -->
        <div v-else class="text-center py-5">
          <div class="card shadow-sm">
            <div class="card-body py-5">
              <i class="bi bi-people display-1 text-muted mb-3"></i>
              <h3 class="text-muted mb-3">No family recipes found</h3>
              <p class="text-muted mb-4">
                Start adding family recipes and preserve your family tradition.
              </p>
              <button @click="showCreateModal = true" class="btn btn-primary btn-lg">
                <i class="bi bi-plus-circle me-2"></i> Add Your First Family Recipe
              </button>
            </div>
          </div>
        </div>

        <!-- Create Recipe Modal -->
        <CreateRecipeModal 
          v-if="showCreateModal" 
          :onClose="closeModal" 
          :onSuccess="fetchFamilyRecipes"
        />

      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import store from '@/store';
import CreateRecipeModal from '@/components/CreateRecipeModal.vue';

export default {
  name: 'FamilyRecipesPage',
  components: {
    CreateRecipeModal
  },
  data() {
    return {
      recipes: [],
      loading: true,
      showCreateModal: false
    };
  },
  created() {
    this.fetchFamilyRecipes();
  },
  methods: {
    async fetchFamilyRecipes() {
      this.loading = true;
      
      try {
        const response = await axios.get(`${store.server_domain}/user/family-recipes`, { withCredentials: true });
        this.recipes = response.data;
      } catch (error) {
        console.error('Error fetching family recipes:', error);
        this.recipes = [];
      } finally {
        this.loading = false;
      }
    },
    closeModal() {
      this.showCreateModal = false;
    }
  }
};
</script>

<style scoped>
.family-recipe-card {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: none;
  border-radius: 15px;
  overflow: hidden;
}

.family-recipe-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.15) !important;
}

.recipe-image-container {
  position: relative;
  overflow: hidden;
}

.recipe-image {
  width: 100%;
  height: 200px;
  object-fit: cover;
  transition: transform 0.3s ease;
  cursor: pointer;
}

.family-recipe-card:hover .recipe-image {
  transform: scale(1.05);
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}

.badge {
  font-size: 0.75rem;
  padding: 0.25rem 0.5rem;
}
</style>
